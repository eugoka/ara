#include "jtag.h"

import jtag;
import bit_field;

#include <stdio.h>
#include <assert.h>
#include <array>
#include <span>
#include <iostream>

int ExitCode;
void check(bool v) {
    if (!v) ExitCode = 1;
}
unsigned bypass_ir_v = 0x1f;
unsigned dtmcs_ir_v = 0x10;
unsigned dmi_ir_v = 0x11;
bit_field::Field singleEmptyBit{1};
bit_field::Field dmi_ir{
    bit_field::Field{5, std::as_writable_bytes(std::span(&bypass_ir_v, 1))},
        bit_field::Field{5, std::as_writable_bytes(std::span(&dmi_ir_v, 1))}};

void submit_dtmcs_scan(struct jtag_driver *driver, bit_field::Field *in, bit_field::Field *out) {
    static bit_field::Field dtmcs_ir{
        bit_field::Field{5, std::as_writable_bytes(std::span(&bypass_ir_v, 1))},
        bit_field::Field{5, std::as_writable_bytes(std::span(&dtmcs_ir_v, 1))}};
    *out = bit_field::Field{bit_field::Field{1}, *out};
    if (in && in != out)
        *in = bit_field::Field{bit_field::Field{1}, *in};

    jtag_submit_loop_and_move(driver, NULL, &singleEmptyBit, 3, 1) /* to SHIFT-IR */;
    jtag_submit_loop_and_move(driver, NULL, &dtmcs_ir, 4, 3) /* to SHIFT-DR */;
    jtag_submit_loop_and_move(driver, in, out, 5, 7) /* to SHIFT-IR */;
    jtag_submit_loop_and_move(driver, NULL, &dmi_ir, 2, 1) /* to RTI */;
}

void submit_select_dmi(struct jtag_driver *driver) {
    jtag_submit_loop_and_move(driver, NULL, &singleEmptyBit, 3, 1) /* to SHIFT-IR */;
    jtag_submit_loop_and_move(driver, NULL, &dmi_ir, 2, 1) /* to RTI */;
}

enum dmi_op {
    DMI_NOP = 0,
    DMI_READ = 1,
    DMI_WRITE = 2,
};

void submit_dmi_scan(struct jtag_driver *driver,
        bit_field::Field &in_data, bit_field::Field in_status,
        unsigned address, enum dmi_op op, unsigned data_out) {
    bit_field::Field out {
        bit_field::Field{1},
        bit_field::Field{2, std::as_writable_bytes(std::span(&op, 1))},
        bit_field::Field{32, std::as_writable_bytes(std::span(&data_out, 1))},
        bit_field::Field{7, std::as_writable_bytes(std::span(&address, 1))}};
    in_data = bit_field::Field{
        bit_field::Field{1},
        in_status,
        in_data,
        bit_field::Field{7} //TODO this is not a constant in general
    };
    jtag_submit_loop_and_move(driver, NULL, &singleEmptyBit, 2, 0) /* to SHIFT-DR */;
    jtag_submit_loop_and_move(driver, &in_data, &out, 2, 1); /* to RTI */;
}

int main() {
    struct jtag_driver *driver = jtag_init();
    jtag_submit_loop_and_move(driver, nullptr, &singleEmptyBit, 5, 0xf) /* to RTI through RESET */;
    jtag_submit_loop_and_move(driver, nullptr, &singleEmptyBit, 2, 0) /* to SHIFT-DR */;
    std::array<unsigned, 2> idcodes;
    bit_field::Field idcodes_in{
        bit_field::Field{32, std::as_writable_bytes(std::span(idcodes).subspan(0, 1))},
        bit_field::Field{32, std::as_writable_bytes(std::span(idcodes).subspan(1, 1))}
    };
    bit_field::Field idcodes_out{64};
    jtag_submit_loop_and_move(driver, &idcodes_in, &idcodes_out, 2, 0x1); /* to RTI */

    unsigned dtmcsVersion = 0, dtmcsAbits = 0;
    bit_field::Field dtmcs_out{32};
    bit_field::Field dtmcs_in{
        bit_field::Field{4, std::as_writable_bytes(std::span(&dtmcsVersion, 1))},
        bit_field::Field{7, std::as_writable_bytes(std::span(&dtmcsAbits, 1))},
        bit_field::Field{21}
    };
    submit_dtmcs_scan(driver, &dtmcs_in, &dtmcs_out);

    jtag_complete(driver);

    submit_select_dmi(driver);

    bit_field::Field dmcontrol_in_req {
        bit_field::Field{32}
    };
    unsigned dmi_status = 0;
    bit_field::Field dmi_status_in {
        bit_field::Field{2, std::as_writable_bytes(std::span(&dmi_status, 1))},
    };
    submit_select_dmi(driver);
    submit_dmi_scan(driver, dmcontrol_in_req, dmi_status_in,
            0x10 /*dmcontrol addr*/, DMI_READ, 0x0);

    unsigned dmcontrol = 0;
    bit_field::Field dmcontrol_in {
        bit_field::Field{32, std::as_writable_bytes(std::span(&dmcontrol, 1))}
    };
    submit_dmi_scan(driver, dmcontrol_in, dmi_status_in,
            0x0, DMI_NOP, 0x0);
        //, std::as_writable_bytes(std::span(&dmstatus, 1))},
#if 0
    dtmcsVersion = 0, dtmcsAbits = 0;
    dtmcs_out = bit_field::Field{32};
    dtmcs_in = bit_field::Field{
        bit_field::Field{4, std::as_writable_bytes(std::span(&dtmcsVersion, 1))},
        bit_field::Field{7, std::as_writable_bytes(std::span(&dtmcsAbits, 1))},
        bit_field::Field{21}
    };
    submit_dtmcs_scan(driver, &dtmcs_in, &dtmcs_out);
#endif
    jtag_complete(driver);
    jtag_deinit(driver);
    check(idcodes[0] == idcodes[1]);
    check(idcodes[0] == 0x07110cfd);
    check(dtmcsVersion == 1);
    check(dtmcsAbits == 7);

    std::cout << "Status: " << dmi_status << " dmcontrol: " << dmcontrol << "\n";
    return ExitCode;
}

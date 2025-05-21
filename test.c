#include "jtag.h"
#include <stdio.h>
#include <assert.h>

void submit_select_dmi(struct jtag_driver *driver) {
    jtag_submit_loop_and_move(driver, 1, NULL, NULL, 3, 1) /* to SHIFT-IR */;
    static const unsigned char dmi_ir[] = {(0x11 & 7) << 5 | 0x1f, 0x11 >> 3};
    jtag_submit_loop_and_move(driver, 5 * 2, NULL, dmi_ir, 2, 1) /* to RTI */;
}

void submit_dtmcs_scan(struct jtag_driver *driver, unsigned char *in, unsigned char *out) {
    jtag_submit_loop_and_move(driver, 1, NULL, NULL, 3, 1) /* to SHIFT-IR */;
    static const unsigned char dtmcs_ir[] = {(0x10 & 7) << 5 | 0x1f, 0x10 >> 3};
    jtag_submit_loop_and_move(driver, 5 * 2, NULL, dtmcs_ir, 4, 3) /* to SHIFT-DR */;
    jtag_submit_loop_and_move(driver, 32, in, out, 5, 7) /* to SHIFT-IR */;
    static const unsigned char dmi_ir[] = {(0x11 & 7) << 5 | 0x1f, 0x11 >> 3};
    jtag_submit_loop_and_move(driver, 5 * 2, NULL, dmi_ir, 2, 1) /* to RTI */;
}

void submit_dmi_scan(struct jtag_driver *driver, unsigned char *in, unsigned char *out) {
    jtag_submit_loop_and_move(driver, 1, NULL, NULL, 2, 0) /* to SHIFT-DR */;
    jtag_submit_loop_and_move(driver, 2 + 32 + 7, in, out, 2, 1);
}

enum dmi_op {
    DMI_NOP = 0,
    DMI_READ = 1,
    DMI_WRITE = 2,
};

unsigned get_dtmcs_version(const unsigned char *buff) {
    return buff[0] & 0xf;
}
unsigned get_dtmcs_abits(const unsigned char *buff) {
    return ((buff[0] & 0xf0) >> 4) | ((buff[1] & 0x3) << 4);
}

void fill_dmi_scan(unsigned char *buff, enum dmi_op op, unsigned int data, unsigned int addr) {
    assert(addr <= 0x5f);
    buff[0] = op;
    buff[0] |= (data & 0x3) << 2;
    buff[1] = (data & (0xfflu << 2)) >> 2;
    buff[3] = (data & (0xfflu << 10)) >> 10;
    buff[4] = (data & (0xfflu << 18)) >> 18;
    buff[5] = (data & (0xfflu << 26)) >> 26;
    buff[5] |= (addr & 0x3) << 2;
    buff[6] = (addr & (0xff << 2)) >> 2;
}

unsigned get_idcode(unsigned char *buff) {
    return buff[0] | buff[1] << 8 | buff[2] << 16 | buff[3] << 24;
}

int main() {
    struct jtag_driver *driver = jtag_init();
    jtag_submit_loop_and_move(driver, 1, NULL, NULL, 5, 0xf) /* to RTI through RESET */;
    jtag_submit_loop_and_move(driver, 1, NULL, NULL, 2, 0) /* to SHIFT-DR */;
    unsigned char out[8] = { 0xff };
    unsigned char in[sizeof(out)] = {0};
    jtag_submit_loop_and_move(driver, sizeof(out) * 8, in, out, 2, 0x1); /* to RTI */
    unsigned char dtmcs_in[4];
    submit_dtmcs_scan(driver, dtmcs_in, NULL);
#if 0
    submit_select_dmi(driver);
    submit_dmi_scan(driver, NULL, NULL);
#endif
    jtag_complete(driver);
    jtag_deinit(driver);
    printf("Expected 0x07110cfd, IDCODE[0]: %x, IDCODE[1]: %x\n", get_idcode(in), get_idcode(in + 4));
    //dtmcontrol=0x5071
    printf("Version: %u, Abits: %u\n", get_dtmcs_version(dtmcs_in), get_dtmcs_abits(dtmcs_in));
}

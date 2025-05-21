struct jtag_driver;
enum jtag_error {
    JTAG_OK,
    JTAG_FAIL,
};
#ifdef __cplusplus
#define JTAG_EXTERN extern "C"
#else
#define JTAG_EXTERN
#endif
JTAG_EXTERN struct jtag_driver *jtag_init();
JTAG_EXTERN enum jtag_error jtag_deinit(struct jtag_driver *);
JTAG_EXTERN void jtag_submit_loop_and_move(struct jtag_driver *ftdi, unsigned int
        loop_count, unsigned char *in, const unsigned char *out, unsigned int n_tms, int tms_bits);
JTAG_EXTERN enum jtag_error jtag_complete(struct jtag_driver *);

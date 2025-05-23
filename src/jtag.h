#ifdef __cplusplus
extern "C" {
#endif
struct jtag_driver;
enum jtag_error {
    JTAG_OK,
    JTAG_FAIL,
};
struct jtag_driver *jtag_init();
enum jtag_error jtag_deinit(struct jtag_driver *);
void jtag_submit_loop_and_move(struct jtag_driver *ftdi, void *in, const void *out, unsigned int n_tms, int tms_bits);
enum jtag_error jtag_complete(struct jtag_driver *);

#ifdef __cplusplus
}
#endif

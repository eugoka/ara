#ifdef __cplusplus
extern "C" {
#endif
unsigned char bit_field_n_bits(const void *);
unsigned char bit_field_n_bytes(const void *);
void bit_field_to_bytes(const void *field, unsigned char src_offset, unsigned char *dst_bytes, unsigned char dst_n_bits, unsigned char dst_offset, unsigned char z_value);
void bit_field_from_bytes(void *field, unsigned char dst_offset, const unsigned char *bytes, unsigned char src_bits, unsigned char src_offset);

#ifdef __cplusplus
}
#endif

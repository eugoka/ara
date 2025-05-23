import bit_field;

#include "bit_field.h"
#include <span>

unsigned char bit_field_n_bits(const void *field) {
    auto f = static_cast<const bit_field::Field *>(field);
    return f->nBits(); }

unsigned char bit_field_n_bytes(const void *field) {
    auto f = static_cast<const bit_field::Field *>(field);
    return f->nBytes(); }

void bit_field_to_bytes(const void *field, unsigned char src_offset,
        unsigned char *bytes, unsigned char dst_n_bits, unsigned char dst_offset,
        unsigned char z_value) {
    auto f = static_cast<const bit_field::Field *>(field);
    return f->to_bytes(src_offset, std::as_writable_bytes(std::span(bytes, f->nBytes())),
                       dst_n_bits, dst_offset, std::byte{z_value});
}

void bit_field_from_bytes(void *field, unsigned char dst_offset, const unsigned char *bytes, unsigned char src_bits, unsigned char src_offset){
    auto f = static_cast< bit_field::Field *>(field);
    return f->from_bytes(dst_offset, std::as_bytes(std::span(bytes, f->nBytes())), src_bits, src_offset);
}

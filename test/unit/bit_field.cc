#include <gtest/gtest.h>
#include <ranges>
#include <array>
#include <span>
import bit_field;
using namespace bit_field;

auto asBytes(auto &val) {
    return std::as_writable_bytes(std::span{&val, 1});
}

TEST(bit_field, copyBits) {
    for (unsigned char offset = 1; offset < CHAR_BIT; ++offset) {
        unsigned src = 1 << offset;
        unsigned dst = 0;
        copyBits(asBytes(src), asBytes(dst), 1, offset);
        EXPECT_EQ(dst, 1);
    }
    for (unsigned char offset = 1; offset < CHAR_BIT; ++offset) {
        unsigned src = 1;
        unsigned dst = 0;
        copyBits(asBytes(src), asBytes(dst), 1, 0, offset);
        EXPECT_EQ(dst, src << offset);
    }
    for (unsigned char offset = 1; offset < CHAR_BIT; ++offset) {
        unsigned src = 3 << offset;
        unsigned dst = 0;
        copyBits(asBytes(src), asBytes(dst), 2, offset);
        EXPECT_EQ(dst, 3) << "at offset " << (int)offset;
    }
    for (unsigned char offset = 0; offset < CHAR_BIT; ++offset) {
        unsigned src = 3 << offset;
        unsigned dst = 0;
        copyBits(asBytes(src), asBytes(dst), 1, offset);
        EXPECT_EQ(dst, 1) << "at offset " << (int)offset;
    }
    for (unsigned char offset = 1; offset < CHAR_BIT; ++offset) {
        unsigned src = 3;
        unsigned dst = 0;
        copyBits(asBytes(src), asBytes(dst), 2, 0, offset);
        EXPECT_EQ(dst, src << offset);
    }
    for (unsigned char offset = 1; offset < CHAR_BIT; ++offset) {
        unsigned src = 0x1ff << offset;
        unsigned dst = 0;
        copyBits(asBytes(src), asBytes(dst), 9, offset);
        EXPECT_EQ(dst, 0x1ff) << "at offset " << (int)offset;
    }
    for (unsigned char offset = 1; offset < CHAR_BIT; ++offset) {
        unsigned src = 0x1ff;
        unsigned dst = 0;
        copyBits(asBytes(src), asBytes(dst), 9, 0, offset);
        EXPECT_EQ(dst, src << offset);
    }
}

TEST(bit_field, byteToFromBytes) {
    unsigned src, dst;
    src = 0xff;
    dst = 0;
    Field{8, asBytes(src)}.to_bytes(0, asBytes(dst), 8);
    EXPECT_EQ(src, dst);
    dst = 0;
    Field{8, asBytes(dst)}.from_bytes(0, asBytes(src), 8);
    EXPECT_EQ(src, dst);
}

TEST(bit_field, bitsToFromBytes) {
    unsigned src = 0xff;
    unsigned dst = 0;
    for (unsigned char offset = 0; offset < CHAR_BIT; ++offset) {
        Field{1, asBytes(src)}.to_bytes(0, asBytes(dst), 1);
        EXPECT_EQ(dst, 1);
        dst = 0;
        Field{1, asBytes(dst)}.from_bytes(0, asBytes(src), 1);
        EXPECT_EQ(dst, 1);
    }
    for (unsigned char offset = 1; offset < CHAR_BIT; ++offset) {
        Field{Field{offset}, Field{1, asBytes(src)}}.to_bytes(0, asBytes(dst), offset + 1);
        EXPECT_EQ(dst, 1 << offset);
        dst = 0;
        Field{Field{offset}, Field{1, asBytes(dst)}}.from_bytes(0, asBytes(src), offset + 1);
        EXPECT_EQ(dst, 1);
    }
}

#include <gtest/gtest.h>
#include <ranges>
import bit_field;
using namespace bit_field;

TEST(bit_field, Get1Bit) {
    std::vector<std::byte> data;
    for (unsigned char offset = 0; offset < CHAR_BIT; ++offset) {
        Field f{1, offset};
        data.resize(f.size());
        f.set_storage(data);
        std::ranges::fill(data, std::byte{});
        EXPECT_FALSE(f.get<bool>()) << "at offset " << (int)offset;
        std::ranges::fill(data, ~std::byte{});
        EXPECT_TRUE(f.get<bool>()) << "at offset " << (int)offset;
    }
}

TEST(bit_field, Get2Bits) {
    std::vector<std::byte> data;
    for (unsigned char offset = 0; offset < CHAR_BIT; ++offset) {
        Field f{2, offset};
        data.resize(f.size());
        f.set_storage(data);
        std::ranges::fill(data, std::byte{});
        EXPECT_EQ(f.get<unsigned>(), 0) << "at offset " << (int)offset;
        std::ranges::fill(data, ~std::byte{});
        EXPECT_EQ(f.get<unsigned>(), 3) << "at offset " << (int)offset;
    }
}

TEST(bit_field, Get10Bits) {
    std::vector<std::byte> data;
    for (unsigned char offset = 0; offset < CHAR_BIT; ++offset) {
        Field f{10, offset};
        data.resize(f.size());
        f.set_storage(data);
        std::ranges::fill(data, std::byte{});
        EXPECT_EQ(f.get<unsigned>(), 0) << "at offset " << (int)offset;
        std::ranges::fill(data, ~std::byte{});
        EXPECT_EQ(f.get<unsigned>(), 0x3ff) << "at offset " << (int)offset;
        std::ranges::fill(data, std::byte{0x55});
        EXPECT_EQ(f.get<unsigned>(), offset % 2 ? 0x2aa : 0x155) << "at offset " << (int)offset;
    }
}

TEST(bit_field, Get32Bits) {
    std::vector<std::byte> data;
    for (unsigned char offset = 0; offset < CHAR_BIT; ++offset) {
        Field f{32, offset};
        data.resize(f.size());
        f.set_storage(data);
        std::ranges::fill(data, std::byte{});
        EXPECT_EQ(f.get<unsigned>(), 0) << "at offset " << (int)offset;
        std::ranges::fill(data, ~std::byte{});
        EXPECT_EQ(f.get<unsigned>(), 0xffffffff) << "at offset " << (int)offset;
        std::ranges::fill(data, std::byte{0x55});
        EXPECT_EQ(f.get<unsigned>(), offset % 2 ? 0xaaaaaaaa : 0x55555555) << "at offset " << (int)offset;
    }
}

TEST(bit_field, MultiField) {
   // Field f{std::vector{Field{32}, Field{10}}};
}

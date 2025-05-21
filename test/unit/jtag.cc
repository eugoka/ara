import jtag;
#include <gtest/gtest.h>
#include <magic_enum/magic_enum_utility.hpp>

using namespace jtag;

TEST(jtag, ResetIn5) {
    magic_enum::enum_for_each<State>([] (auto state) {
            constexpr Path path{state, State::kTLR};
            EXPECT_LE(path.length_, 5);
            EXPECT_EQ(path.tms_bits_, (1 << path.length_) - 1);
    });
}

TEST(jtag, SelectDRtoShiftDR) {
    constexpr Path path{State::kSelectDR, State::kShiftDR};
    EXPECT_EQ(path.length_, 2);
    EXPECT_EQ(path.tms_bits_, 0);
}

TEST(jtag, Exit1DRtoRTI) {
    constexpr Path path{State::kExit1DR, State::kRTI};
    EXPECT_EQ(path.length_, 2);
    EXPECT_EQ(path.tms_bits_, 1);
}

TEST(jtag, SelectDRtoShiftIR) {
    constexpr Path path{State::kSelectDR, State::kShiftIR};
    EXPECT_EQ(path.length_, 3);
    EXPECT_EQ(path.tms_bits_, 1);
}

TEST(jtag, Exit1IRtoRTI) {
    constexpr Path path{State::kExit1IR, State::kRTI};
    EXPECT_EQ(path.length_, 2);
    EXPECT_EQ(path.tms_bits_, 1);
}

TEST(jtag, Exit1IRtoShiftDR) {
    constexpr Path path{State::kExit1IR, State::kShiftDR};
    EXPECT_EQ(path.length_, 4);
    EXPECT_EQ(path.tms_bits_, 3);
}

TEST(jtag, Exit1DRtoShiftIR) {
    Path path{State::kExit1DR, State::kShiftIR};
    EXPECT_EQ(path.length_, 5);
    EXPECT_EQ(path.tms_bits_, 7);
}

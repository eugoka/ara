import arm_semihosting;
#include <gmock/gmock.h>
#include <gtest/gtest.h>
using namespace arm_semihosting;
using namespace testing;

class MockTarget {
   public:
    using Pointer = unsigned int;
    MOCK_METHOD(enum Operations, GetSemihostingOperation, ());
    MOCK_METHOD(Pointer, GetSemihostingParameter, ());
    MOCK_METHOD(std::byte, ReadMemory, (Pointer _));
};

TEST(arm_semihosting, Writec) {
    MockTarget mock_target;
    std::string debug_out;
    const auto address = 0xdeadbeef;
    const auto the_char = 'x';
    SemihostingHandler handler =
        SemihostingHandler{std::stringstream(debug_out)};
    Expectation get_op = EXPECT_CALL(mock_target, GetSemihostingOperation())
                             .WillOnce(Return(Operations::kSysWritec));
    Expectation get_param = EXPECT_CALL(mock_target, GetSemihostingParameter())
                                .WillOnce(Return(address));
    EXPECT_CALL(mock_target, ReadMemory(address))
        .After(get_op, get_param)
        .WillOnce(Return(std::byte{the_char}));
    handler.handle(mock_target);
}

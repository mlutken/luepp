#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../state_machine.hpp"

using namespace testing;

class StateMachineUnitTest : public testing::Test
{
public:
    StateMachineUnitTest()  = default;

    ~StateMachineUnitTest() override = default;

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

// ------------------------------
// --- PUBLIC functions tests ---
// ------------------------------

TEST_F(StateMachineUnitTest, test_test)
{
    EXPECT_EQ(6, 6);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

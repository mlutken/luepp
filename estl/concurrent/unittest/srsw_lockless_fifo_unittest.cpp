#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../srsw_lockless_fifo.hpp"

using namespace testing;

class LocklessFifoUnitTest : public testing::Test
{
public:
    LocklessFifoUnitTest()  = default;

    ~LocklessFifoUnitTest() override = default;

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

TEST_F(LocklessFifoUnitTest, test_test)
{
    EXPECT_EQ(6, 6);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../srsw_fifo.hpp"

using namespace testing;

class SRSWFifoUnitTest : public testing::Test
{
public:
    SRSWFifoUnitTest()  = default;

    ~SRSWFifoUnitTest() override = default;

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

TEST_F(SRSWFifoUnitTest, test_test)
{
    EXPECT_EQ(6, 6);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

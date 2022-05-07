#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../srsw_fifo.hpp"

using namespace testing;

class SRSWFifoUnitTest : public testing::Test
{
};

// ------------------------------
// --- PUBLIC functions tests ---
// ------------------------------
using int_fifo = estl::srsw_fifo<int>;

TEST_F(SRSWFifoUnitTest, test_basic)
{
    int_fifo f(4);
    EXPECT_EQ(static_cast<size_t>(0u), f.size());
    EXPECT_EQ(4u, f.buffer_size());
    EXPECT_EQ(true, f.empty());
    EXPECT_EQ(false, f.full());
    {
        bool ok = f.push(10);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(1u, f.size());
        EXPECT_EQ(10u, f.front());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }
    {
        bool ok = f.push(20);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(2u, f.size());
        EXPECT_EQ(10u, f.front());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }
    {
        bool ok = f.push(30);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(3u, f.size());
        EXPECT_EQ(10u, f.front());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(true, f.full());
    }
    {
        bool ok = f.push(40);
        EXPECT_EQ(false, ok);
        EXPECT_EQ(3u, f.size());
        EXPECT_EQ(10u, f.front());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(true, f.full());
    }
    {
        bool ok = f.pop();
        EXPECT_EQ(true, ok);
        EXPECT_EQ(2u, f.size());
        EXPECT_EQ(20u, f.front());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }

    {
        bool ok = f.push(40);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(3u, f.size());
        EXPECT_EQ(20u, f.front());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(true, f.full());
    }


    {
        bool ok = f.pop();
        EXPECT_EQ(true, ok);
        EXPECT_EQ(2u, f.size());
        EXPECT_EQ(30u, f.front());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }

    {
        bool ok = f.pop();
        EXPECT_EQ(true, ok);
        EXPECT_EQ(1u, f.size());
        EXPECT_EQ(40u, f.front());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }

    {
        bool ok = f.pop();
        EXPECT_EQ(true, ok);
        EXPECT_EQ(static_cast<size_t>(0u), f.size());
        EXPECT_EQ(true, f.empty());
        EXPECT_EQ(false, f.full());
    }

}

TEST_F(SRSWFifoUnitTest, test_size)
{
    int_fifo f(4);
    EXPECT_EQ(static_cast<size_t>(0u), f.size());
    f.push(10);
    EXPECT_EQ(1u, f.size());
    f.push(20);
    EXPECT_EQ(2u, f.size());
    f.push(30);
    EXPECT_EQ(3u, f.size());
    f.push(40);
    EXPECT_EQ(3u, f.size());
    f.pop();
    EXPECT_EQ(2u, f.size());
    f.push(40);
    EXPECT_EQ(3u, f.size());
    f.pop();
    EXPECT_EQ(2u, f.size());
    f.pop();
    EXPECT_EQ(1u, f.size());
    f.pop();
    EXPECT_EQ(static_cast<size_t>(0u), f.size());
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

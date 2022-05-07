#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <mutex>

#include "../mutex.hpp"


using namespace testing;

class MutexUnitTest : public testing::Test
{
};

// ------------------------------
// --- PUBLIC functions tests ---
// ------------------------------
using my_mutex = estl::mutex;
//using my_mutex = std::mutex;

TEST_F(MutexUnitTest, mutex_test_basic)
{
    my_mutex m;
    {
        bool ok = m.try_lock();
        EXPECT_EQ(true, ok);
    }
    {
        bool ok = m.try_lock();
        EXPECT_EQ(false, ok);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

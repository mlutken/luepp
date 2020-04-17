#include <stdexcept>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../atomic.hpp"
#include <vector>
#include <iostream>

using namespace testing;
using namespace estl;

class AtomicUnitTest : public testing::Test
{
public:
    AtomicUnitTest()  = default;

    ~AtomicUnitTest() override = default;

    void SetUp() override
    {
    }
    void TearDown() override;

};

void AtomicUnitTest::TearDown()
{
}

// ----------------------------------------
// --- Simple helper test element class ---
// ----------------------------------------

// -------------------
// -- Constructors ---
// -------------------

TEST_F(AtomicUnitTest, default_constructor)
{
    using atomic_int = atomic<int>;
    atomic_int a(2);
    EXPECT_EQ(2, a);

    a = 3;
    EXPECT_EQ(3, a);

}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

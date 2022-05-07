#include <stdexcept>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <iostream>
#include <atomic/atomic.hpp>

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
    using atomic_int = estl::atomic<int>;
    atomic_int a(2);
    EXPECT_EQ(2, a);
}

TEST_F(AtomicUnitTest, assignment)
{
    using atomic_int = estl::atomic<int>;
    atomic_int a(2);
    EXPECT_EQ(2, a);

    a = 3;
    EXPECT_EQ(3, a);
}

TEST_F(AtomicUnitTest, increment)
{
    using atomic_int = estl::atomic<int>;
    atomic_int a(2);
    EXPECT_EQ(2, a);
    a++;
    EXPECT_EQ(3, a);
    ++a;
    EXPECT_EQ(4, a);

    a += 5;
    EXPECT_EQ(9, a);
}

TEST_F(AtomicUnitTest, decrement)
{
    using atomic_int = estl::atomic<int>;
    atomic_int a(10);
    EXPECT_EQ(10, a);
    a--;
    EXPECT_EQ(9, a);

    --a;
    EXPECT_EQ(8, a);

    a -= 5;
    EXPECT_EQ(3, a);
}

TEST_F(AtomicUnitTest, bitwise_and)
{
    using atomic_int = estl::atomic<int>;
    atomic_int a(0b1110);
    EXPECT_EQ(0b1110, a);

    a &= 0b0111;
    EXPECT_EQ(0b0110, a);
}

TEST_F(AtomicUnitTest, bitwise_or)
{
    using atomic_int = estl::atomic<int>;
    atomic_int a(0b1110);
    EXPECT_EQ(0b1110, a);

    a |= 0b0111;
    EXPECT_EQ(0b1111, a);
}

TEST_F(AtomicUnitTest, bitwise_xor)
{
    using atomic_int = estl::atomic<int>;
    atomic_int a(0b1110);
    EXPECT_EQ(0b1110, a);

    a ^= 0b0111;
    EXPECT_EQ(0b1001, a);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

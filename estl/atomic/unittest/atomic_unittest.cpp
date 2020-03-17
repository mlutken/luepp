#include <stdexcept>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../atomic.hpp"
#include <vector>

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
    EXPECT_EQ(2, 2);

//     shared_ptr<int, 10> v;
//     EXPECT_TRUE(v.empty());
//     EXPECT_EQ(static_cast<size_t>(0u), v.size());
//     EXPECT_EQ(10u, v.capacity());
//     EXPECT_EQ(10u, v.max_size());
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

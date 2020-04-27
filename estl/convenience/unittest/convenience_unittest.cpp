#include <stdexcept>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <convenience/convenience.hpp>
#include <vector>

using namespace testing;
using namespace estl;

class ConvenienceUnitTest : public testing::Test
{
public:
    ConvenienceUnitTest()  = default;

    ~ConvenienceUnitTest() override = default;

    void SetUp() override
    {
    }
    void TearDown() override;

};

void ConvenienceUnitTest::TearDown()
{
}

// ----------------------------------------
// --- Simple helper test element class ---
// ----------------------------------------

struct Rect {
    Rect() = default;
    Rect(unsigned w, unsigned h)
        : width_(w),
          height_(h)
    {
    }

    unsigned width_ = 0;
    unsigned height_ = 0;
};


inline bool operator== (const Rect& lhs, const Rect& rhs) {
    return  lhs.width_ == rhs.width_ &&
            lhs.height_ == rhs.height_;
}



// -------------------
// -- Constructors ---
// -------------------

TEST_F(ConvenienceUnitTest, default_constructor)
{
    EXPECT_EQ(2, 2);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

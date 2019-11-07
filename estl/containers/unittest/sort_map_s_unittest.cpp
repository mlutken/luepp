#include <stdexcept>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../sort_map_s.hpp"
#include <vector>

using namespace testing;
using namespace estl;

class SortMapSUnitTest : public testing::Test
{
public:
    SortMapSUnitTest()  = default;

    ~SortMapSUnitTest() override = default;

    void SetUp() override
    {
    }
    void TearDown() override;

};

void SortMapSUnitTest::TearDown()
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

TEST_F(SortMapSUnitTest, default_constructor)
{
    using MyMap = estl::sort_map_s<std::string, std::string, 30>;
    MyMap m;
    EXPECT_EQ(0u, m.size());
}

TEST_F(SortMapSUnitTest, basic_operations)
{
    using MyMap = estl::sort_map_s<std::string, std::string, 30>;
    MyMap m;

    m["key1"] = "val1";
    EXPECT_EQ(1u, m.size());
    EXPECT_EQ("val1", m.begin()->second);
    EXPECT_EQ("val1", m["key1"]);

    m["key2"] = "val2";
    EXPECT_EQ(2u, m.size());
    EXPECT_EQ("val2", m.begin()->second);
    EXPECT_EQ("val2", m["key2"]);
    EXPECT_EQ("val1", m["key1"]);

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

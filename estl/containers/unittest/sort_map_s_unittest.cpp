#include <stdexcept>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../sort_map_s.hpp"
#include <vector>
#include <string>

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
    EXPECT_EQ(static_cast<size_t>(0u), m.size());
}

TEST_F(SortMapSUnitTest, basic_operations)
{
    using MyMap = estl::sort_map_s<std::string, std::string, 30>;
    MyMap m;

    EXPECT_EQ(static_cast<size_t>(0u), m.size());

    m["key_100"] = "val_100";
    EXPECT_EQ(1u, m.size());
    EXPECT_EQ("val_100", m["key_100"]);

    m["key_090"] = "val_090";
    EXPECT_EQ(2u, m.size());
    EXPECT_EQ("val_090", m["key_090"]);

    m["key_080"] = "val_080";
    EXPECT_EQ(3u, m.size());
    EXPECT_EQ("val_080", m["key_080"]);

    m["key_180"] = "val_180";
    EXPECT_EQ(4u, m.size());
    EXPECT_EQ("val_180", m["key_180"]);

    auto it = m.find("key_090");
    EXPECT_EQ("val_090", it->second);
}

TEST_F(SortMapSUnitTest, insert)
{
    using MyMap = estl::sort_map_s<std::string, std::string, 30>;
    MyMap m;

    {
        MyMap::value_type elem("key_100", "val_100");
        const auto it = m.insert(elem);
        EXPECT_EQ(1u, m.size());
        EXPECT_EQ("val_100", it->second);
        EXPECT_EQ("val_100", m["key_100"]);
    }

    {
        MyMap::value_type elem("key_090", "val_090");
        const auto it = m.insert(elem);
        EXPECT_EQ(2u, m.size());
        EXPECT_EQ("val_090", it->second);
        EXPECT_EQ("val_090", m["key_090"]);
    }

}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

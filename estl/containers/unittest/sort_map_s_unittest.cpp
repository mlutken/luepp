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

TEST_F(SortMapSUnitTest, erase)
{
    using MyMap = estl::sort_map_s<std::string, std::string, 30>;
    MyMap m;
    m.insert({"key_010", "val_010"});
    m.insert({"key_020", "val_020"});
    m.insert({"key_030", "val_030"});
    m.insert({"key_040", "val_040"});
    m.insert({"key_050", "val_050"});
    m.insert({"key_060", "val_060"});
    m.insert({"key_070", "val_070"});
    m.insert({"key_080", "val_080"});
    m.insert({"key_000", "val_000"});
    m.insert({"key_100", "val_100"});
    m.insert({"key_110", "val_110"});
    m.insert({"key_090", "val_090"});

//    MyMap m {
//        {"key_010", "val_010"},
//        {"key_020", "val_020"},
//        {"key_030", "val_030"},
//        {"key_040", "val_040"},
//        {"key_050", "val_050"},
//        {"key_060", "val_060"},
//        {"key_070", "val_070"},
//        {"key_080", "val_080"},
//        {"key_000", "val_000"},
//        {"key_100", "val_100"},
//        {"key_110", "val_110"},
//        {"key_090", "val_090"}
//    };

    EXPECT_EQ(12u, m.size());
    EXPECT_EQ("val_010", m["key_010"]);
    EXPECT_EQ("val_020", m["key_020"]);
    EXPECT_EQ("val_030", m["key_030"]);
    EXPECT_EQ("val_040", m["key_040"]);
    EXPECT_EQ("val_050", m["key_050"]);
    EXPECT_EQ("val_060", m["key_060"]);
    EXPECT_EQ("val_070", m["key_070"]);
    EXPECT_EQ("val_080", m["key_080"]);
    EXPECT_EQ("val_000", m["key_000"]);
    EXPECT_EQ("val_100", m["key_100"]);
    EXPECT_EQ("val_110", m["key_110"]);
    EXPECT_EQ("val_090", m["key_090"]);

    {
        const auto it = m.find("key_060");
        EXPECT_EQ("key_060", it->first);
        EXPECT_EQ("val_060", it->second);
        m.erase(it);
        EXPECT_EQ(11u, m.size());
    }

    EXPECT_EQ("val_010", m["key_010"]);
    EXPECT_EQ("val_020", m["key_020"]);
    EXPECT_EQ("val_030", m["key_030"]);
    EXPECT_EQ("val_040", m["key_040"]);
    EXPECT_EQ("val_050", m["key_050"]);
    EXPECT_EQ("val_070", m["key_070"]);
    EXPECT_EQ("val_080", m["key_080"]);
    EXPECT_EQ("val_000", m["key_000"]);
    EXPECT_EQ("val_100", m["key_100"]);
    EXPECT_EQ("val_110", m["key_110"]);
    EXPECT_EQ("val_090", m["key_090"]);

    {
        const auto it = m.find("key_110");
        EXPECT_EQ("key_110", it->first);
        EXPECT_EQ("val_110", it->second);
        m.erase(it);
        EXPECT_EQ(10u, m.size());
    }

    EXPECT_EQ("val_010", m["key_010"]);
    EXPECT_EQ("val_020", m["key_020"]);
    EXPECT_EQ("val_030", m["key_030"]);
    EXPECT_EQ("val_040", m["key_040"]);
    EXPECT_EQ("val_050", m["key_050"]);
    EXPECT_EQ("val_070", m["key_070"]);
    EXPECT_EQ("val_080", m["key_080"]);
    EXPECT_EQ("val_000", m["key_000"]);
    EXPECT_EQ("val_100", m["key_100"]);
    EXPECT_EQ("val_090", m["key_090"]);

    {
        const auto it = m.find("key_010");
        EXPECT_EQ("key_010", it->first);
        EXPECT_EQ("val_010", it->second);
        m.erase(it);
        EXPECT_EQ(9u, m.size());
    }

    EXPECT_EQ("val_020", m["key_020"]);
    EXPECT_EQ("val_030", m["key_030"]);
    EXPECT_EQ("val_040", m["key_040"]);
    EXPECT_EQ("val_050", m["key_050"]);
    EXPECT_EQ("val_070", m["key_070"]);
    EXPECT_EQ("val_080", m["key_080"]);
    EXPECT_EQ("val_000", m["key_000"]);
    EXPECT_EQ("val_100", m["key_100"]);
    EXPECT_EQ("val_090", m["key_090"]);

}

#if (CXX_STANDARD != 98)

TEST_F(SortMapSUnitTest, initializer_construct)
{
    using MyMap = estl::sort_map_s<std::string, std::string, 30>;

    MyMap m {
        {"key_010", "val_010"},
        {"key_020", "val_020"},
        {"key_030", "val_030"},
        {"key_040", "val_040"},
        {"key_050", "val_050"},
        {"key_060", "val_060"},
        {"key_070", "val_070"},
        {"key_080", "val_080"},
        {"key_000", "val_000"},
        {"key_100", "val_100"},
        {"key_110", "val_110"},
        {"key_090", "val_090"}
    };

    EXPECT_EQ(12u, m.size());
    EXPECT_EQ("val_010", m["key_010"]);
    EXPECT_EQ("val_020", m["key_020"]);
    EXPECT_EQ("val_030", m["key_030"]);
    EXPECT_EQ("val_040", m["key_040"]);
    EXPECT_EQ("val_050", m["key_050"]);
    EXPECT_EQ("val_060", m["key_060"]);
    EXPECT_EQ("val_070", m["key_070"]);
    EXPECT_EQ("val_080", m["key_080"]);
    EXPECT_EQ("val_000", m["key_000"]);
    EXPECT_EQ("val_100", m["key_100"]);
    EXPECT_EQ("val_110", m["key_110"]);
    EXPECT_EQ("val_090", m["key_090"]);
}

#endif // (CXX_STANDARD != 98)

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

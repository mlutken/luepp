#include <gtest/gtest.h>
#include "strings/split.h"
#include "strings/trim.h"

using namespace testing;
using namespace lue::strings;

class TrimTest : public testing::Test
{
public:
    TrimTest()  = default;

    ~TrimTest() override = default;

    void SetUp() override
    {
    }
    void TearDown() override;

};

void TrimTest::TearDown()
{
}

// -------------------
// -- Constructors ---
// -------------------


TEST_F(TrimTest, trim_simple)
{
    std::string abc {" \n\tabc"};
    std::string_view abc_viev {abc};
    EXPECT_EQ("abc", ltrim(abc));
    EXPECT_EQ("abc", ltrim(abc_viev));
    EXPECT_EQ("abc", ltrim_view(abc_viev));
    EXPECT_EQ("abc", ltrim_view(" abc"));
    EXPECT_EQ("abc", ltrim_view(abc));

    EXPECT_EQ("abc", rtrim("abc "));
    EXPECT_EQ("abc", rtrim("abc \n\t"));
    EXPECT_EQ("abc", rtrim_view("abc \n\t"));

    EXPECT_EQ("abc", trim("  abc "));
    EXPECT_EQ("abc", trim("\t \n abc \n\t"));
    EXPECT_EQ("abc", trim_view("\t \n abc \n\t"));
}

TEST_F(TrimTest, split_at_substring)
{
    std::string s = "scott>=tigers>=mushroom";
    std::string delimiter = ">=";

    const auto v = split(s, delimiter); // ["scott", "tigers", "mushroom"]
    EXPECT_EQ(3, v.size());
    EXPECT_EQ("scott", v[0]);
    EXPECT_EQ("tigers", v[1]);
    EXPECT_EQ("mushroom", v[2]);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

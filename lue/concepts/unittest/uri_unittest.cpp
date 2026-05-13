#include <gtest/gtest.h>
#include "concepts/uri.h"

using namespace testing;
using namespace lue::concepts;

class UriUnitTest : public testing::Test
{
public:
    UriUnitTest()  = default;

    ~UriUnitTest() override = default;

    void SetUp() override
    {
    }
    void TearDown() override;

};

void UriUnitTest::TearDown()
{
}

// -------------------
// -- Constructors ---
// -------------------

TEST_F(UriUnitTest, default_constructor)
{
    EXPECT_EQ("", uri{}.string());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

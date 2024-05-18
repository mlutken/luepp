#include <gtest/gtest.h>

#include "../commands.h"


using namespace testing;

class CommandsUnitTest : public testing::Test
{
};

// ------------------------------
// --- PUBLIC functions tests ---
// ------------------------------

TEST_F(CommandsUnitTest, fixme_add_test)
{
    EXPECT_EQ(true, true);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include "../my_syslog.h"
#include <my_base_module/my_base_module_mock.h>
#include <cesl_gmock/cesl_gmock_free_func.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// GMOCK_C_FUNC_IMPLEMENTATION(my_base_module_)

using namespace testing;

class MySyslogUnitTest : public testing::Test
{
public:
//     StrictMock<my_base_module_Mock>      my_base_module_mock;

    MySyslogUnitTest()  {}

    ~MySyslogUnitTest() {}

    /*  Warning: SetUp will be called before each TEST_F but not for every
     *           loop if the test contains for-loops.
     *           Each loop will reset the expected counters though.
     */
    void SetUp()
    {
    }

    void TearDown()
    {
    }
};

// ------------------------------
// --- PUBLIC functions tests ---
// ------------------------------

TEST_F(MySyslogUnitTest, create_syslog)
{
    EXPECT_EQ(1,1);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

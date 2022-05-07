#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cesl_gmock/cesl_gmock_free_func.h>

#include "../my_base_module.h"

// ---------------------------------------------
// --- SYSTEM Mocking for my_base_module ---
// ---------------------------------------------
#define FUNCTION_LIST_my_base_module_system_(F, ...) \
    F(1, sqrt,              double,     double,      ## __VA_ARGS__)

GMOCK_FREE_FUNC_DECLARATION(my_base_module_system_)
GMOCK_C_FUNC_IMPLEMENTATION(my_base_module_system_)

using namespace testing;

class MyBaseModuleUnitTest : public testing::Test
{
public:
    StrictMock<my_base_module_system_Mock>      my_base_module_system_mock;

    MyBaseModuleUnitTest()  {}

    ~MyBaseModuleUnitTest() override {}

    /*  Warning: SetUp will be called before each TEST_F but not for every
     *           loop if the test contains for-loops.
     *           Each loop will reset the expected counters though.
     */
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

// ------------------------------
// --- PUBLIC functions tests ---
// ------------------------------

TEST_F(MyBaseModuleUnitTest, int_multiply__basic_test)
{
    EXPECT_EQ(6, int_multiply(2,3));
}

TEST_F(MyBaseModuleUnitTest, vec3_len2__basic_test)
{
    EXPECT_EQ(29, vec3_len2(2,3,4));
}

TEST_F(MyBaseModuleUnitTest, vec3_len__basic_test)
{
    EXPECT_CALL(my_base_module_system_mock,
                sqrt(29) )
            .WillOnce(Return(5.385164807))
            ;
    EXPECT_EQ(5, vec3_len(2,3,4));
}

// --------------------------------------
// --- PRIVATE Helper functions tests ---
// --------------------------------------

//TEST_F(MyBaseModuleUnitTest, int_square_root__basic_test)
//{
//    EXPECT_CALL(my_base_module_system_mock,
//                sqrt(5.0f) )
//            .WillOnce(Return(2.23606797f))
//            ;
//    EXPECT_EQ(2, int_square_root(5));
//}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

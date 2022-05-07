#include "../my_dependant_module.h"
#include <my_base_module/my_base_module_mock.h>
#include <cesl_gmock/cesl_gmock_free_func.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

GMOCK_C_FUNC_IMPLEMENTATION(my_base_module_)

using namespace testing;

class MyDependantModuleUnitTest : public testing::Test
{
public:
    StrictMock<my_base_module_Mock>      my_base_module_mock;

    MyDependantModuleUnitTest()  {}

    ~MyDependantModuleUnitTest() {}

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

 TEST_F(MyDependantModuleUnitTest, vec3_len__basic_test)
 {
     EXPECT_CALL(my_base_module_mock,
                 int_multiply(2, 3) )
             .WillOnce(Return(6))
             ;
     EXPECT_CALL(my_base_module_mock,
                 vec3_len2(2, 3, 6) )
             .WillOnce(Return(49))
             ;
     EXPECT_CALL(my_base_module_mock,
                 vec3_len(2, 3, 49) )
             .WillOnce(Return(49))
             ;
     EXPECT_EQ(49, my_funky_calculation(2,3));
 }


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

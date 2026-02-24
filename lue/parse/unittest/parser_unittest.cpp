#include <stdexcept>
#include <gtest/gtest.h>


#include <parse/parser.h>

using namespace testing;
using namespace lue;

class ParserUnitTest : public testing::Test
{
public:
    ParserUnitTest()  = default;

    ~ParserUnitTest() override = default;

    void SetUp() override
    {
    }
    void TearDown() override;

};

void ParserUnitTest::TearDown()
{

}

// ----------------------------------------
// --- Simple helper test element class ---
// ----------------------------------------




// -------------------
// -- Constructors ---
// -------------------

TEST_F(ParserUnitTest, default_constructor)
{
    // EXPECT_EQ(static_cast<size_t>(0u), m.size());
    EXPECT_EQ(true, true);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

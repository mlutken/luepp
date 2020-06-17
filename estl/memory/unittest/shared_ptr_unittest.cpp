#include <stdexcept>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../shared_ptr.hpp"
#include <memory>

using namespace testing;
using namespace estl;

class SharedPtrUnitTest : public testing::Test
{
};


// ----------------------------------------
// --- Simple helper test element class ---
// ----------------------------------------

class MyClass {
public:
    MyClass () {
    }
    MyClass (const MyClass& other) : val_(other.val_) {
    }

    MyClass (MyClass&& other) noexcept : val_(other.val_) {
        std::cout << "MyClass(MyClass&& other) MOVE CONSTRUCTOR: " << val_ << " this: " << this <<  std::endl;
    }

    MyClass& operator= (const MyClass& other) {
        val_ = other.val_;
        return *this;
    }

    MyClass& operator= (MyClass&& other) noexcept {
        val_ = other.val_;
        return *this;
    }

    MyClass (int val) : val_(val) {
    }

    ~MyClass() {
    }


    int val() const
    {
        return val_;
    }
    void val(int val)
    {
        val_ = val;
    }
private:
    int val_ = -99;
};


// -------------------
// -- Constructors ---
// -------------------
template <class T>
//using shared_ptr_t = std::shared_ptr<T>;
using shared_ptr_t = estl::shared_ptr<T>;

TEST_F(SharedPtrUnitTest, constructor)
{
    shared_ptr_t<MyClass> mc1;
    EXPECT_EQ(mc1.use_count(), 0);
}


TEST_F(SharedPtrUnitTest, DISABLED__basic_test)
{
    shared_ptr_t<MyClass> mc1;
    EXPECT_EQ(mc1.use_count(), 0);
    {
        shared_ptr_t<MyClass> mc2 = shared_ptr_t<MyClass>(new MyClass());
        mc2->val(12);
        mc1 = mc2;
        EXPECT_EQ(mc1.use_count(), 2);
        EXPECT_EQ(mc2.use_count(), 2);
    }
    EXPECT_EQ(mc1.use_count(), 1);
}

TEST_F(SharedPtrUnitTest, DISABLED__weak_ptr_test)
{
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

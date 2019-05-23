#include <iostream>
#include <vector>
#include <array>
#include <containers/vector_s.hpp>
#include <containers/myarray.hpp>

struct MyClass {
    MyClass () {
        std::cout << "MyClass DEFAULT CONSTRUCTOR: " << val_ << " this: " << this <<  std::endl;
        //  = default;  TODO: We should not need this
    }
    MyClass (const MyClass& other) : val_(other.val_) {
        std::cout << "MyClass(MyClass& other) COPY CONSTRUCTOR: " << val_ << " this: " << this <<  std::endl;
    }

    MyClass (MyClass&& other) noexcept : val_(other.val_) {
        std::cout << "MyClass(MyClass&& other) MOVE CONSTRUCTOR: " << val_ << " this: " << this <<  std::endl;
    }

    MyClass& operator= (const MyClass& other) {
        val_ = other.val_;
        std::cout << "MyClass COPY assignment(const MyClass& other)" << val_ << " this: " << this <<  std::endl;
        return *this;
    }

    MyClass& operator= (MyClass&& other) noexcept {
        val_ = other.val_;
        std::cout << "MyClass MOVE assignment(MyClass&& other)" << val_ << " this: " << this <<  std::endl;
        return *this;
    }

    MyClass (int val) : val_(val) {
        std::cout << "MyClass(int val) CONSTRUCTOR: " << val_ << " this: " << this <<  std::endl;
    }

    ~MyClass() {
        std::cout << "~MyClass! DESTRUCTOR: " << val_ << " this: " << this <<  std::endl;
    }

    int val_ = -99;
};

inline std::ostream& operator<<(std::ostream& os, const MyClass& mc)
{
    os << mc.val_;
    return os;
}


using namespace std;



int main()
{
    cout << "--- vector_s playground ---\n";

    //    cout << "1" << endl;

    {
        cout << "--- In scope: Creating vector ---" << endl;
        //        cas::vector_s<MyClass, 4> v = {2, 3, 6};
        //        std::array<MyClass, 4> v = {2, 3, 6};
        //        std::vector<MyClass> v = {2, 3, 6};

        //        cas::vector_s<MyClass, 3> v = {1, 2};
        //        std::array<MyClass, 3> v = {1, 2};
        //        std::vector<MyClass> v = {1, 2};

        //        cas::vector_s<MyClass, 3> v = {1, 2};
        //        std::array<MyClass, 3> v = {1, 2};
        //        std::vector<MyClass> v = {1, 2};

//        MyClass v[2] = {1};

//        cas::vector_s<MyClass, 2> v {1};
//        std::vector<MyClass> v = {1};
//        std::array<MyClass, 2> v = {1};
//        boost::array<MyClass, 2> v = {1};
//        cas::myarray<MyClass, 2> v = {1};

        estl::vector_s<MyClass, 10> v;
//        std::vector<MyClass> v;
//        v.reserve(10);
//        v.emplace_back(1);
//        v.emplace(v.begin(),2);
        v.emplace(v.begin(),1);
//         MyClass mc1(10);
//        v.at(0) = mc1;
//        std::array<MyClass, 2> v;
        cout << "--- In scope: After create ---" << endl;
        cout << "--- In scope: acces v.back(): " << *std::begin(v) << " ---" << endl;
        cout << "---                 v.size(): " << v.size() << " ---" << endl;
//        v.clear();
//        std::vector<MyClass> v2;
//        v2.emplace_back(100);
//        v.assign({20});

//        cout << "--- In scope: acces v2.back: " << v2.back() << " ---" << endl;

        cout << "--- In scope: Scope exit ---" << endl;
    }
    cout << "--- after scope ---" << endl;

//    MyClass mc1 = 10;
//    MyClass mc2;
//    mc2 = std::move(mc1);

    return 0;
}

#include <iostream>
#include <vector>
#include <array>
#include <memory/shared_ptr.hpp>

class MyClass {
public:
    MyClass () {
        std::cerr << "MyClass DEFAULT CONSTRUCTOR: " << val_ << " this: " << this <<  std::endl;
        //  = default;  TODO: We should not need this
    }
    MyClass (const MyClass& other) : val_(other.val_) {
        std::cerr << "MyClass(MyClass& other) COPY CONSTRUCTOR: " << val_ << " this: " << this <<  std::endl;
    }

    MyClass (MyClass&& other) noexcept : val_(other.val_) {
        std::cerr << "MyClass(MyClass&& other) MOVE CONSTRUCTOR: " << val_ << " this: " << this <<  std::endl;
    }

    MyClass& operator= (const MyClass& other) {
        val_ = other.val_;
        std::cerr << "MyClass COPY assignment(const MyClass& other)" << val_ << " this: " << this <<  std::endl;
        return *this;
    }

    MyClass& operator= (MyClass&& other) noexcept {
        val_ = other.val_;
        std::cerr << "MyClass MOVE assignment(MyClass&& other)" << val_ << " this: " << this <<  std::endl;
        return *this;
    }

    MyClass (int val) : val_(val) {
        std::cerr << "MyClass(int val) CONSTRUCTOR: " << val_ << " this: " << this <<  std::endl;
    }

    ~MyClass() {
        std::cerr << "~MyClass! DESTRUCTOR: " << val_ << " this: " << this <<  std::endl;
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

inline std::ostream& operator<<(std::ostream& os, const MyClass& mc)
{
    os << mc.val();
    return os;
}


using namespace std;


//template <class T>
//using shared_ptr_t = std::shared_ptr<T>;
//template <class T>
//using weak_ptr_t = std::weak_ptr<T>;

template <class T>
using shared_ptr_t = estl::shared_ptr<T>;
template <class T>
using weak_ptr_t = estl::weak_ptr<T>;

int main()
{

    {
        shared_ptr_t<MyClass> mc1 = shared_ptr_t<MyClass>(new MyClass(10));
        auto mc2 = mc1;
        cerr << "mc1.use_clount(2) : " << mc1.use_count() << ",  mc1.value: " << mc1->val() <<  "\n";
        cerr << "mc2.use_count(2) : " << mc2.use_count() << ",  mc2.value: " << mc2->val() <<  "\n";
        mc1.reset(new MyClass(20));
        cerr << "mc1.use_count(1) : " << mc1.use_count() << ",  mc1.value: " << mc1->val() <<  "\n";
        cerr << "mc2.use_count(1) : " << mc2.use_count() << ",  mc2.value: " << mc2->val() <<  "\n";
    }

    shared_ptr_t<MyClass> s_ptr = shared_ptr_t<MyClass>(new MyClass());
    return 0;
}


int main3()
{

    {
        shared_ptr_t<MyClass> mc1;
        mc1 = shared_ptr_t<MyClass>(new MyClass());
        auto mc2 = mc1;
        mc1.reset(new MyClass());
    }

    weak_ptr_t<MyClass> w_ptr;
    {
        shared_ptr_t<MyClass> s_ptr = shared_ptr_t<MyClass>(new MyClass());
        w_ptr = s_ptr;
        cerr << "w_ptr.use_count(1) : " << w_ptr.use_count() << "\n";
        auto s_ptr2 = w_ptr.lock();
        if (static_cast<bool>(s_ptr2) == true) {
            cerr << "w_ptr.lock() succeeded as expected\n";
        }
        cerr << "w_ptr.use_count(2)  : " << w_ptr.use_count() << "\n";
        cerr << "s_ptr.use_count(2)  : " << s_ptr.use_count() << "\n";
        cerr << "s_ptr2.use_count(2) : " << s_ptr2.use_count() << "\n";
        std::cout << "w_ptr.use_count() inside scope: " << w_ptr.use_count() << "\n";
    }
    cerr << "w_ptr.use_count(0)  : " << w_ptr.use_count() << "\n";
    auto s_ptr3 = w_ptr.lock();
    if (static_cast<bool>(s_ptr3) == false) {
        cerr << "w_ptr.lock() failed as expected\n";
    }


    return 0;
}


int main2()
{
    cerr << "--- shared_ptr playground 1 ---\n";
    weak_ptr_t<MyClass> w_ptr;
    {
        shared_ptr_t<MyClass> s_ptr = shared_ptr_t<MyClass>(new MyClass());
        w_ptr = s_ptr;
        cerr << "w_ptr.use_count() inside scope: " << w_ptr.use_count() << "\n";
        shared_ptr_t<MyClass> s_ptr2 = w_ptr.lock();
        if (s_ptr2) {
            s_ptr->val(3);
            cerr << "weak_ptr lock succeded inside scope w_ptr.use_count() : " << w_ptr.use_count() << "\n";
            cerr << "weak_ptr lock succeded inside scope s_ptr.use_count() : " << s_ptr.use_count() << "\n";
            cerr << "weak_ptr lock succeded inside scope s_ptr2.use_count(): " << s_ptr2.use_count() << "\n";
        }
    }
    cerr << "w_ptr.use_count() outside scope: " << w_ptr.use_count() << '\n';
    shared_ptr_t<MyClass> s_ptr3 = w_ptr.lock();
    if (s_ptr3) {
        cerr << "lock outside w_ptr.use_count() : " << w_ptr.use_count() << " val: " << s_ptr3->val() <<  "\n";
        cerr << "lock outside s_ptr2.use_count(): " << s_ptr3.use_count() << "\n";
    }

    return 0;
}

int main1()
{
    cerr << "--- shared_ptr playground 1 ---\n";
    shared_ptr_t<MyClass> mc1;
    {
        shared_ptr_t<MyClass> mc2 = shared_ptr_t<MyClass>(new MyClass());
        mc2->val(12);
        mc1 = mc2;
        cerr << "mc1.get() : " << mc1.get() << "mc1.use_count():" << mc1.use_count() << " \n";
    }
    cerr << "--- shared_ptr playground 2 ---\n";

    cerr << "mc1.get() : " << mc1.get() << "mc1.use_count():" << mc1.use_count() << " \n";

    if (mc1) {
        cerr << "mc1->val(): " << mc1->val() << "\n";
    }

    cerr << "--- shared_ptr playground 3: mc1 ---\n";

    return 0;
}





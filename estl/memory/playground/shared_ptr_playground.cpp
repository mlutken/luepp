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


template <class T>
//using shared_ptr_t = std::shared_ptr<T>;
using shared_ptr_t = estl::shared_ptr<T>;

int main()
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





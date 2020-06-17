#include <iostream>
#include <vector>
#include <array>
#include <memory/shared_ptr.hpp>

class MyClass {
public:
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



int main()
{
    cerr << "--- shared_ptr playground 1 ---\n";
    estl::shared_ptr<MyClass> mc1;
    {
        estl::shared_ptr<MyClass> mc2 = estl::shared_ptr<MyClass>(new MyClass());
        mc2->val(12);
        mc1 = mc2;
    }
    cerr << "--- shared_ptr playground 2 ---\n";

    cerr << "mc1.get() : " << mc1.get() << "\n";

    cerr << "--- shared_ptr playground 3 ---\n";

    return 0;
}





#include <nestle_default_config.h>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <containers/vector_s.hpp>

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

template <class CONTAINER>
void debug_print(const CONTAINER& container)
{
    for(const auto& elem : container) {
        std::cerr << elem << ", ";
    }
    std::cerr << "\n";
}


using namespace std;
using namespace estl;

#if (CXX_STANDARD != 98)
void main__cpp11();
#endif


int main()
{
    cout << "--- vector_s playground ---\n";

#if (CXX_STANDARD != 98)
    main__cpp11();
#endif

}
#if (CXX_STANDARD != 98)
void main__cpp11()
{
    using myvec_t = estl::vector_s<int, 50>;
    using stringvec_t = estl::vector_s<std::string, 50>;
//    using stringvec_t = std::vector<std::string>;

    myvec_t v{7,3,5,1,20,9,8};

    debug_print(v);
    std::sort(v.begin(), v.end(), std::greater<int>());
    debug_print(v);

    stringvec_t vs{"07","03","05","01","20","09","08"};

    debug_print(vs);
    std::sort(vs.begin(), vs.end(), std::greater<std::string>());
    debug_print(vs);
}

#endif

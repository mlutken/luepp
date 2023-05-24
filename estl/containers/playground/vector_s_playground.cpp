#include <luepp_platform_definitions.h>
#include <luepp_default_config.h>
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
        std::cout << elem << ", ";
    }
    std::cout << std::endl;
}


using namespace std;
using namespace estl;

void main__cpp11();
void main__constxepr();


int main()
{
    cout << "--- vector_s playground ---\n";
    main__cpp11();
    main__constxepr();
    std::cout << std::endl;
}

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

constexpr size_t N = 10;

using intvec_t = estl::vector_s<size_t, N>;

template<size_t N>
struct Table
{
    constexpr Table() : values()
    {
        for (size_t i = 0u; i < N; ++i)
        {
            values[i][0] = i;
            values[i][1] = i * i * i;
        }
    }
    size_t values[N][2u];
};

template<size_t N>
struct Vec
{
    constexpr Vec() : data_()
    {
        for (size_t i = 0u; i < N; ++i)
        {
            data_[i] = i;
        }
    }
    size_t          size_ = 0u;
    char            data_[N*sizeof(size_t)];
//    size_t*         data_ptr_ = static_cast<size_t*>( static_cast<void*>(&data_[0]) );
    //    size_t&         data_ptr_ = *(static_cast<size_t*>( static_cast<void*>(&data_[0]) ));
};

//constexpr int create_int_vec()
//{
//    constexpr auto constexpr_int_vec = Vec<3>();
//    int sum = 0;
//    for (int i = 0; i < N; ++i) {
//        sum += i;
//    }

//    return sum;
//}

void main__constxepr()
{
//    constexpr int size = create_int_vec();
//    static_assert(size == (N*(N-1))/2);
//    std::cout << "sum: " << size << "\n";

//    constexpr auto v = estl::vector_s<int, N>();
    constexpr auto a = Table<10>();
    for (auto x : a.values) {
        std::cout << "f(" << x[0] << ") = " << x[1] << '\n';
    }

}



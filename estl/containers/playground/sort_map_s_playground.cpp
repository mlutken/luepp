#include <iostream>
#include <vector>
#include <array>
#include <containers/sort_map_s.hpp>

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
    cout << "--- sort_map_s playground ---\n";
    estl::vector_s<std::string, 12> v;
    estl::vector_s<std::pair<std::string, std::string>, 12> vp;
//    std::vector<std::string> v;


    std::string s1("sds"s);
    v.push_back("sds"s);
    v.push_back("sds2"s);
    v.push_back("sds3"s);

//    auto p1 = make_pair("1", "2");
//    vp.push_back({"fg","frgfd"});

    using MyMap = estl::sort_map_s<std::string, std::string, 30>;
    MyMap m;

//    m.insert({"food"s, "mad"s});
    m["goat"] = "ged";

    cout << "m['goat'] => " << m["goat"] << "\n";


    return 0;
}

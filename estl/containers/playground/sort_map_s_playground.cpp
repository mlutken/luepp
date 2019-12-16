#include <iostream>
#include <vector>
#include <array>
#include <string>
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

    using MyMap = estl::sort_map_s<std::string, std::string, 30>;
    MyMap m;

    //m.insert({"food"s, "mad"s});
    m["goat"] = "ged";
    m["horse"] = "hest";

    cout << "m.size()" << m.size() << "\n";
    cout << "m['goat'] => " << m["goat"] << "\n";
    cout << "m['horse'] => " << m["horse"] << "\n";


//    std::vector<std::string> strings = {"hello", "nihao", "byebye", "yo"};
//    estl::vector_s<std::string, 20> strings = {"hello", "nihao", "byebye", "yo"};

    // cars.sort(std::less<std::string>()); // compiles fine and produce a sorted list

//    std::sort(strings.begin(), strings.end(), std::less<std::string>() );

    std::cerr << "Printing Map\n";
    for (auto it = m.begin(); it != m.end(); ++it) {
        std::cerr << (*it).first << " => " << (*it).second << "\n";
    }

    std::cerr << "\n";


    return 0;
}

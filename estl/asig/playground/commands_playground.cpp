#include <iostream>
#include <vector>
#include <string>

#include <asig/command_queue.h>

using namespace std;
using namespace estl;


struct MyClass
{
    explicit MyClass(std::string name) { name_ = std::move(name); }

    void member_function(int some_number)
    {
        cerr << name_ << "::member_function(" << some_number << ")\n";
    }

    std::string name_;
};

void free_function(int some_number)
{
    cerr << "Hello free_function(" << some_number << ")\n";;
}

command_queue queue1{256};
MyClass my_class{"MyClass"};

int main()
{
    cerr << "--- commands playground ---\n";
    cerr << "queue1.capacity()   : "  << queue1.capacity() << "\n";
    cerr << "queue1.size()       : "  << queue1.size() << "\n";

    queue1.push([](){cerr << "Hello from push\n"; });
    queue1.call(free_function, 12);
    queue1.call(&MyClass::member_function, &my_class, 23);


    while (!queue1.empty()) {
        queue1.execute_next();
    }
    return 0;
}

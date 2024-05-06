#include <iostream>
#include <vector>
#include <string>

#include <asig/command_queue.h>

using namespace std;
using namespace estl;


struct Thread1Class
{
    explicit Thread1Class(std::string name) { name_ = std::move(name); }

    void member_function(int some_number)
    {
        cerr << name_ << "::member_function(" << some_number << ")\n";

    }

    int mul2(int some_number)
    {
        cerr << name_ << "::mul2(" << some_number << ")\n";
        return 2*some_number;
    }

    std::string name_;
};

struct Thread2Class
{
    explicit Thread2Class(std::string name) { name_ = std::move(name); }

    void callback_fun(int some_number)
    {
        cerr << name_ << "::callback(" << some_number << ")\n";
    }

    std::string name_;
};

void free_function(int some_number)
{
    cerr << "Hello free_function(" << some_number << ")\n";;
}

void free_callback(int some_number)
{
    cerr << "Hello free_callback(" << some_number << ")\n";;
}

command_queue queue1{256};
Thread1Class thread_1_class{"Thread 1 Class"};
Thread2Class thread_2_class{"Thread 2 Class"};

int main()
{
    cerr << "--- commands playground ---\n";
    cerr << "queue1.capacity()   : "  << queue1.capacity() << "\n";
    cerr << "queue1.size()       : "  << queue1.size() << "\n";

    queue1.push_call_void([](){cerr << "Hello from push\n"; });
    queue1.call_void(free_function, 12);
    queue1.call_void(&Thread1Class::member_function, &thread_1_class, 23);


    auto command_fn = [=]() -> int {
        return thread_1_class.mul2(25);
    };

    auto result_callback_fn = [=](const int& cmd_return_value){
        free_callback(cmd_return_value);
    };
    queue1.push_call<int>(std::move(command_fn),  std::move(result_callback_fn));


    queue1.call<int>(free_callback, &Thread1Class::mul2, &thread_1_class, 30);
    queue1.call_memfun<int>(&Thread2Class::callback_fun, &thread_2_class, &Thread1Class::mul2, &thread_1_class, 40);


    while (!queue1.empty()) {
        queue1.execute_next();
    }
    return 0;
}

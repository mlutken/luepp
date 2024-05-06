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

void test_variadic();

int main()
{
    test_variadic();
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

    queue1.call_test<int>(free_callback, &Thread1Class::mul2, &thread_1_class, 31);
    queue1.call_test<int>(&Thread2Class::callback_fun, &thread_2_class, &Thread1Class::mul2, &thread_1_class, 41);

    while (!queue1.empty()) {
        queue1.execute_next();
    }
    return 0;
}

//////////////////

#include <iostream>
#include <tuple>

template < typename ... >
struct two_impl {};

// Base case
template < typename F,
         typename ...Bs >
struct two_impl < F, std::tuple <>, std::tuple< Bs... > >  {
    void operator()(F f, Bs... bs) {
        std::invoke(f, bs...);
    }
};

// Recursive case
template < typename F,
         typename A,
         typename ...As,
         typename ...Bs >
struct two_impl < F, std::tuple< A, As... >, std::tuple< Bs...> >  {
    void operator()(F f, A a, As... as, Bs... bs) {
        auto impl = two_impl < F, std::tuple < As... >, std::tuple < Bs..., A> >();
        impl(f, as..., bs..., a);
    }
};

template < typename F, typename ...Ts >
void two(F f, Ts ...ts) {
    auto impl = two_impl< F, std::tuple < Ts... >, std::tuple <> >();
    impl(f, ts...);
}

// ----------------------------------
template < typename ... >
struct call_impl {};

// Base case
template<typename ReturnType, class ResultMemberCallable, class ResultClassObject>
struct call_impl <ReturnType, ResultMemberCallable, ResultClassObject>  {
    void operator()(ResultMemberCallable member_fun, ResultClassObject call_class_instance) {
        // std::invoke(f, bs...);
    }
};

          //   ( ResultMemberCallable result_callback_fun, ResultClassObject result_class_instance,
          //  CommandCallable command_fun, CommandArgs... command_args
          // )



struct Test {
    void fun1(int i) {
        std::cerr << "fun1(" << i << ")\n";
    }
    int cmd1(int i) {
        std::cerr << "cmd1(" << i << ")\n";
        return 2*i;
    }

    void fun3(int i, float f, double d) {
        std::cerr << i << std::endl << f << std::endl << d << std::endl;
    }

    void operator()(int i, float f, double d) {
        std::cerr << i << std::endl << f << std::endl << d << std::endl;
    }
};

void test_variadic()
{
    Test test;

    cerr << "--- test_variadic ---\n";
    two(&Test::fun1, &test, 2);
    two(&Test::cmd1, &test, 5);
    cerr << "\n";
}

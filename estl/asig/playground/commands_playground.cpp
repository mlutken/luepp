#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include <asig/commands.h>
#include <asig/command_queue.h>

using namespace std;
using namespace estl;
using namespace std::chrono;
using namespace std::chrono_literals;

void free_function(int some_number)
{
    cerr << "Hello free_function(" << some_number << ")\n";;
}

void free_callback(int some_number)
{
    cerr << "Hello free_callback(" << some_number << ")\n";;
}

struct Thread1Class
{
    explicit Thread1Class(commands& cc, std::string name) : command_center_(cc), name_ (std::move(name))  {}
    ~Thread1Class()  { is_running_ = false; }

    void start  () {
        is_running_ = false;
        thread_ = std::make_unique<std::thread>(&Thread1Class::thread_function, this);
        cerr << "Starting thread ' " << name_ << "' ID: " << thread_->get_id() << "\n";
        thread_->detach();
    }
    void stop           () { is_running_ = false; }
    bool is_running     () const { return is_running_; }

    // -------------------------
    // --- Command functions ---
    // --------------------------
    void member_function(int some_number)
    {
        cerr << name_ << "::member_function(" << some_number << ")\n";

    }

    int mul2(const int some_number)
    {
        cerr << name_ << "::mul2(" << some_number << ")\n";
        return 2*some_number;
    }

private:

    // -----------------------------
    // --- Thread main functions ---
    // -----------------------------
    void thread_function()
    {
        command_center_.register_receiver(this);

        is_running_ = true;
        const auto end_time = steady_clock::now() + 6s;
        while(is_running_ && (steady_clock::now() < end_time) ) {
            std::this_thread::sleep_for(1s);
            work_function();
        }
    }

    void work_function()
    {
        cerr << "In '" << name_ << "'\n";
    }


    // ------------------------
    // --- Member variables ---
    // ------------------------
    commands&                       command_center_;
    std::string                     name_               {};
    std::shared_ptr<command_queue>  command_queue_      {nullptr};
    std::atomic<bool>               is_running_         {false};
    std::unique_ptr<std::thread>    thread_             {nullptr};
};


struct Thread2Class
{
    explicit Thread2Class(commands& cc, std::string name) : command_center_(cc), name_ (std::move(name))  {}
    ~Thread2Class()  { is_running_ = false; }

    void start  () {
        is_running_ = false;
        thread_ = std::make_unique<std::thread>(&Thread2Class::thread_function, this);
        cerr << "Starting thread ' " << name_ << "' ID: " << thread_->get_id() << "\n";
        thread_->detach();
    }
    void stop           () { is_running_ = false; }
    bool is_running     () const { return is_running_; }

    // -----------------
    // --- Callbacks ---
    // -----------------
    void callback_fun(int some_number)
    {
        cerr << name_ << "::callback(" << some_number << ")\n";
    }

private:

    // -----------------------------
    // --- Thread main functions ---
    // -----------------------------
    void thread_function()
    {
        command_center_.register_receiver(this);

        is_running_ = true;
        const auto end_time = steady_clock::now() + 6s;
        while(is_running_ && (steady_clock::now() < end_time) ) {
            std::this_thread::sleep_for(1s);
            work_function();
        }
    }

    void work_function()
    {
        cerr << "In '" << name_ << "'\n";
    }


    // ------------------------
    // --- Member variables ---
    // ------------------------
    commands&                       command_center_;
    std::string                     name_               {};
    std::shared_ptr<command_queue>  command_queue_      {nullptr};
    std::atomic<bool>               is_running_         {false};
    std::unique_ptr<std::thread>    thread_             {nullptr};
};


commands command_center{64};
command_queue queue1{256};
Thread1Class thread_1_class{command_center, "Thread 1 Class"};
Thread2Class thread_2_class{command_center, "Thread 2 Class"};

int main()
{

    thread_1_class.start();
    thread_2_class.start();
    while (!(thread_1_class.is_running() && thread_2_class.is_running())) {
        this_thread::sleep_for(1ms);
    }
    command_center.dbg_print_receivers();

    cerr << "--- commands playground, Main thread ID: " << this_thread::get_id() << "---\n";
    cerr << "command_center.queues_size()       : "  << command_center.queues_size() << "\n";
    cerr << "command_center.queues_count()      : "  << command_center.queues_count() << "\n";
    cerr << "command_center.receivers_count()   : "  << command_center.receivers_count() << "\n";
    cerr << "queue1.capacity()                  : "  << queue1.capacity() << "\n";
    cerr << "queue1.size()                      : "  << queue1.size() << "\n";

    // queue1.push_call_void([](){cerr << "Hello from push\n"; });
    // queue1.call_void(free_function, 12);
    // queue1.call_void(&Thread1Class::member_function, &thread_1_class, 23);

    // auto command_fn = [=]() -> int {
    //     return thread_1_class.mul2(25);
    // };

    // auto result_callback_fn = [=](const int& cmd_return_value){
    //     free_callback(cmd_return_value);
    // };
    // queue1.push_call<int>(std::move(command_fn),  std::move(result_callback_fn));

    // queue1.callback_free<int>(free_callback, &Thread1Class::mul2, &thread_1_class, 30);
    // queue1.callback<int>(&Thread2Class::callback_fun, &thread_2_class, &Thread1Class::mul2, &thread_1_class, 40);
    cerr << "thread_1_class       : "  << &thread_1_class  << "\n";
    cerr << "thread_2_class       : "  << &thread_2_class  << "\n";

    queue1.callback<int>(&Thread2Class::callback_fun, &thread_2_class, &Thread1Class::mul2, &thread_1_class, 40);
    queue1.callback<int>(free_callback, &Thread1Class::mul2, &thread_1_class, 41);
    queue1.callback<int>([](int val) {cerr << "Lambda callback: " << val << "\n";}, &Thread1Class::mul2, &thread_1_class, 42);


    // queue1.call_test<int>    (free_callback, &Thread1Class::mul2, &thread_1_class, 31);
    // queue1.call_test<int>(&Thread2Class::callback_fun, &thread_2_class, &Thread1Class::mul2, &thread_1_class, 41);

    while (!queue1.empty()) {
        queue1.execute_next();
    }

    std::this_thread::sleep_for(4s);
    // thread_1_class.stop();
    thread_2_class.stop();
    return 0;
}


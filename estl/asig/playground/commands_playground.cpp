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

commands command_center{64};
command_queue queue1{256};

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

    int square_me(const int& some_number)
    {
        const auto squared = some_number*some_number;
        cerr  << "{" << this_thread::get_id() << "} " << name_ << "::square_me(" << some_number << ") => " << squared << "\n";
        return squared;
    }

private:

    // -----------------------------
    // --- Thread main functions ---
    // -----------------------------
    void thread_function()
    {
        command_center_.register_command_receiver(this);
        command_queue_ = command_center_.get_receiver_queue();

        is_running_ = true;
        const auto end_time = steady_clock::now() + 6s;
        while(is_running_ && (steady_clock::now() < end_time) ) {
            std::this_thread::sleep_for(900ms);
            // cerr << "{" << this_thread::get_id() << "}  In '" << name_ << "'  Processing commands\n";
            while (!command_queue_->empty()) {
                command_queue_->execute_next();
            }

            idle_work_function();
        }
    }

    void idle_work_function()
    {
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

Thread1Class thread_1_class{command_center, "Thread 1 Class"};

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
    void callback_fun1(int squared_number)
    {
        cerr << "{" << this_thread::get_id() << "} " << name_ << "::callback_fun1(" << squared_number << ")\n";
    }

    void callback_fun2(int32_t cmd_seq_num, int squared_number)
    {
        cerr << "{" << this_thread::get_id() << "} " << name_ << "::callback_fun2(" << squared_number << ") , cmd_seq_num: " << cmd_seq_num << "\n";
    }

private:

    // -----------------------------
    // --- Thread main functions ---
    // -----------------------------
    void thread_function()
    {
        command_center_.register_command_receiver(this);
        command_queue_ = command_center_.get_receiver_queue();

        is_running_ = true;
        const auto end_time = steady_clock::now() + 6s;
        while(is_running_ && (steady_clock::now() < end_time) ) {
            std::this_thread::sleep_for(10ms);
            // cerr << "{" << this_thread::get_id() << "}  In '" << name_ << "'  Processing commands\n";
            while (!command_queue_->empty()) {
                command_queue_->execute_next();
            }

            idle_work_function();
        }
    }

    void idle_work_function()
    {
        static int square_me_parameter = 0;
        static int32_t cmd_seq_num = 0;
        square_me_parameter += 10;
        cmd_seq_num += 1;

        if (cmd_seq_num < 5) {
            cerr << "From {" << this_thread::get_id() << "} / '" << name_ << "'  Calling Thread1Class::square_me(" << square_me_parameter << ") , cmd_seq_num: " << cmd_seq_num << "\n";
            // command_center_.send_response<int>(&Thread2Class::callback_fun1, this, &Thread1Class::square_me, &thread_1_class, square_me_parameter);
            command_center_.send_response<int>(&Thread2Class::callback_fun2, this, cmd_seq_num, &Thread1Class::square_me, &thread_1_class, square_me_parameter);
        }
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


Thread2Class thread_2_class{command_center, "Thread 2 Class"};

void threads_test()
{

    thread_1_class.start();
    thread_2_class.start();
    while (!(thread_1_class.is_running() && thread_2_class.is_running())) {
        this_thread::sleep_for(1ms);
    }
    command_center.dbg_print_command_receivers();

    cerr << "--- commands playground, Main thread ID: " << this_thread::get_id() << "---\n";
    cerr << "command_center.queues_size()       : "  << command_center.queues_size() << "\n";
    cerr << "command_center.queues_count()      : "  << command_center.queues_count() << "\n";
    cerr << "command_center.receivers_count()   : "  << command_center.receivers_count() << "\n";

    cerr << "thread_1_class       : "  << &thread_1_class  << "\n";
    cerr << "thread_2_class       : "  << &thread_2_class  << "\n";


    std::this_thread::sleep_for(4s);
    thread_1_class.stop();
    thread_2_class.stop();
}

void simple_test()
{
    cerr << "thread_1_class       : "  << &thread_1_class  << "\n";
    cerr << "thread_2_class       : "  << &thread_2_class  << "\n";
    cerr << "queue1.capacity()    : "  << queue1.capacity() << "\n";
    cerr << "queue1.size()        : "  << queue1.size() << "\n";
    queue1.push([](){cerr << "Hello from push\n"; });


    // queue1.send(free_function, 12);
    // queue1.send(&Thread1Class::member_function, &thread_1_class, 23);

    // auto command_fn = [=]() -> int {
    //     return thread_1_class.square_me(25);
    // };

    // auto result_callback_fn = [=](const int& cmd_return_value){
    //     free_callback(cmd_return_value);
    // };
    // queue1.push_call<int>(std::move(command_fn),  std::move(result_callback_fn));

    // queue1.callback_free<int>(free_callback, &Thread1Class::square_me, &thread_1_class, 30);
    // queue1.callback<int>(&Thread2Class::callback_fun, &thread_2_class, &Thread1Class::square_me, &thread_1_class, 40);

    // command_center.command_cb<int>(&Thread2Class::callback_fun, &thread_2_class, &Thread1Class::square_me, &thread_1_class, 40);

    // queue1.send_callback<int>(&Thread2Class::callback_fun, &thread_2_class, &Thread1Class::square_me, &thread_1_class, 40);

    // queue1.send_callback<int>(free_callback, &Thread1Class::square_me, &thread_1_class, 41);
    // queue1.send_callback<int>([](int val) {cerr << "Lambda callback: " << val << "\n";}, &Thread1Class::square_me, &thread_1_class, 42);

    while (!queue1.empty()) {
        queue1.execute_next();
    }
}


int main()
{
    threads_test();
    // simple_test();
    return 0;
}

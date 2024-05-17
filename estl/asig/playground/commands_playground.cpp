#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <mutex>
#include <unordered_map>
#include <typeinfo>

#include <asig/commands.h>
#include <asig/command_queue.h>

using namespace std;
using namespace estl::asig;
using namespace std::chrono;
using namespace std::chrono_literals;

commands command_center{64};
command_queue queue1{256};

std::mutex thread_name_map_mutex{};
std::unordered_map<std::thread::id, std::string> thread_name_map;

void add_thread_name(const std::string& name, std::thread::id thread_id = std::this_thread::get_id()) {
    std::scoped_lock<std::mutex> lock(thread_name_map_mutex);
    thread_name_map[thread_id] = name;
}

std::string thread_name(std::thread::id thread_id = std::this_thread::get_id()) {
    std::scoped_lock<std::mutex> lock(thread_name_map_mutex);
    return thread_name_map[thread_id];
}


int qube_me_free_fun(int some_number)
{
    const auto qubed = some_number*some_number*some_number;
    cerr  << "{" << thread_name() << "} qube_me_free_fun(" << some_number << ") => " << qubed << "\n";
    return qubed;
}

void free_function(int some_number)
{
    cerr << "{" << thread_name() << "} " << "Hello free_function(" << some_number << ")\n";;
}

void free_callback(int some_number)
{
    cerr << "{" << thread_name() << "} " << "Hello free_callback(" << some_number << ")\n";
}

void free_callback_seq_num(int some_number, int32_t cmd_seq_num)
{
    cerr << "{" << thread_name() << "} free_callback_seq_num(" << some_number << ") , cmd_seq_num: " << cmd_seq_num << "\n";
}


struct Thread1Class
{
    explicit Thread1Class(commands& cc, std::string name) : command_center_(cc), name_ (std::move(name))  {}
    ~Thread1Class()  {
        is_running_ = false;
        thread_->join();
    }

    void start  () {
        is_running_ = false;
        thread_ = std::make_unique<std::thread>(&Thread1Class::thread_function, this);
        cerr << "Starting thread ' " << name_ << "' ID: " << thread_->get_id() << "\n";
    }

    std::thread::id     thread_id      () const { return thread_->get_id(); }
    void                stop           ()       { is_running_ = false;      }
    bool                is_running     () const { return is_running_;       }

    // -------------------------
    // --- Command functions ---
    // --------------------------
    void mem_fun_no_params()
    {
        cerr << "{" << thread_name() << "} " << name_ << "::mem_fun_no_params()\n";

    }

    void member_function(int some_number)
    {
        cerr << "{" << thread_name() << "} " << name_ << "::member_function(" << some_number << ")\n";

    }

    int square_me(const int& some_number)
    {
        const auto squared = some_number*some_number;
        cerr  << "{" << thread_name() << "} " << name_ << "::square_me(" << some_number << ") => " << squared << "\n";
        return squared;
    }

private:

    // -----------------------------
    // --- Thread main functions ---
    // -----------------------------
    void thread_function()
    {
        add_thread_name("T1");
        command_center_.register_command_receiver(this);

        is_running_ = true;
        const auto end_time = steady_clock::now() + 6s;
        while(is_running_ && (steady_clock::now() < end_time) ) {
            std::this_thread::sleep_for(900ms);
            // cerr << "{" << thread_name() << "}  In '" << name_ << "'  Processing commands\n";
            command_center_.execute_all_for_this_thread();

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
    std::atomic<bool>               is_running_         {false};
    std::unique_ptr<std::thread>    thread_             {nullptr};
};

Thread1Class thread_1{command_center, "Thread 1"};

struct Thread2Class
{
    explicit Thread2Class(commands& cc, std::string name) : command_center_(cc), name_ (std::move(name))  {}
    ~Thread2Class()  {
        is_running_ = false;
        thread_->join();
    }

    void start  () {
        is_running_ = false;
        thread_ = std::make_unique<std::thread>(&Thread2Class::thread_function, this);
        cerr << "Starting thread ' " << name_ << "' ID: " << thread_->get_id() << "\n";
    }

    std::thread::id     thread_id      () const { return thread_->get_id(); }
    void                stop           ()       { is_running_ = false;      }
    bool                is_running     () const { return is_running_;       }

    // -----------------
    // --- Callbacks ---
    // -----------------
    void callback_squared(int squared_number)
    {
        cerr << "{" << thread_name() << "} " << name_ << "::callback_squared(" << squared_number << ")\n";
    }

    void callback_qubed(int qubed_number)
    {
        cerr << "{" << thread_name() << "} " << name_ << "::callback_qubed(" << qubed_number << ")\n";
    }

    void callback_squared_seq_num(int squared_number, int32_t cmd_seq_num)
    {
        cerr << "{" << thread_name() << "} " << name_ << "::callback_squared_seq_num(" << squared_number << ") , cmd_seq_num: " << cmd_seq_num << "\n";
    }

    void callback_qubed_seq_num(int qubed_number, int32_t cmd_seq_num)
    {
        cerr << "{" << thread_name() << "} " << name_ << "::callback_qubed_seq_num(" << qubed_number << ") , cmd_seq_num: " << cmd_seq_num << "\n";
    }

private:

    // -----------------------------
    // --- Thread main functions ---
    // -----------------------------
    void thread_function()
    {
        add_thread_name("T2");
        command_center_.register_command_receiver(this);

        is_running_ = true;
        const auto end_time = steady_clock::now() + 6s;
        while(is_running_ && (steady_clock::now() < end_time) ) {
            std::this_thread::sleep_for(10ms);
            // cerr << "{" << thread_name() << "}  In '" << name_ << "'  Processing commands\n";
            command_center_.execute_all_for_this_thread();

            idle_work_function();
        }
    }

    void idle_work_function()
    {
        static int square_me_param = 0;
        static int32_t static_cmd_seq_num = 0;
        square_me_param += 10;
        static_cmd_seq_num += 1;
        const int32_t seq_num = static_cmd_seq_num;


        if (static_cmd_seq_num < 5) {
            cerr << "From {" << thread_name() << "} SEQUENCE NUMBER Response Thread1Class::square_me(" << square_me_param << ") , cmd_seq_num: " << seq_num << "\n";

            // ---------------------------------------------
            // --- Normal callback and response examples ---
            // ---------------------------------------------
//            command_center_.send_callback<int>(&Thread2Class::callback_squared, this, &Thread1Class::square_me, &thread_1, square_me_param);
//            command_center_.send_response<int>(&Thread2Class::callback_squared, this, &Thread1Class::square_me, &thread_1, square_me_param);


            // ---------------------------------------------------
            // --- Using sequence numbers in callback/response ---
            // ---------------------------------------------------
//            command_center_.send_callback<int>(&Thread2Class::callback_squared_seq_num, this, seq_num, &Thread1Class::square_me, &thread_1, square_me_param);
            command_center_.send_response<int>(&Thread2Class::callback_squared_seq_num, this, seq_num, &Thread1Class::square_me, &thread_1, square_me_param);

            // ------------------------------------------------------------------
            // --- Lambda callback/response (with sequence number in capture) ---
            // ------------------------------------------------------------------
//            auto lambda_callback_squared = [=](const int& squared_number)  {
//                cerr << "{" << thread_name() << "} lambda_callback_squared(" << squared_number << "), seq number: " << seq_num << "\n";
//            };
//            command_center.send_callback<int>(lambda_callback_squared, &Thread1Class::square_me, &thread_1, square_me_param);
//            command_center.send_response<int>(lambda_callback_squared, &Thread1Class::square_me, &thread_1, square_me_param);
        }
    }


    // ------------------------
    // --- Member variables ---
    // ------------------------
    commands&                       command_center_;
    std::string                     name_               {};
    std::atomic<bool>               is_running_         {false};
    std::unique_ptr<std::thread>    thread_             {nullptr};
};


Thread2Class thread_2{command_center, "Thread 2"};


void threads_test()
{

    thread_1.start();
    thread_2.start();
    while (!(thread_1.is_running() && thread_2.is_running())) {
        this_thread::sleep_for(1ms);
    }
    command_center.dbg_print_command_receivers();


    command_center.send(&Thread1Class::mem_fun_no_params, &thread_1);
    command_center.send(&Thread1Class::member_function, &thread_1, 12);
    int32_t cmd_seq_num = 1;
    command_center.send_callback<int>(&Thread2Class::callback_squared_seq_num, &thread_2, cmd_seq_num, &Thread1Class::square_me, &thread_1, 7);
    command_center.send_callback<int>(&Thread2Class::callback_squared, &thread_2, &Thread1Class::square_me, &thread_1, 8);

    auto lambda_callback_squared = [=](int squared_number)  {
        cerr << "{" << thread_name() << "} lambda_callback_squared(" << squared_number << ")\n";
    };
    command_center.send_callback<int>(lambda_callback_squared, &Thread1Class::square_me, &thread_1, 8);

    cerr << "\n--- commands playground, Main thread ID: " << thread_name() << "---\n";
    cerr << "command_center.queues_size()       : "  << command_center.queues_size() << "\n";
    cerr << "command_center.queues_count()      : "  << command_center.queues_count() << "\n";
    cerr << "command_center.receivers_count()   : "  << command_center.receivers_count() << "\n";

    cerr << "thread_1_class       : "  << &thread_1  << "\n";
    cerr << "thread_2_class       : "  << &thread_2  << "\n";


    std::this_thread::sleep_for(4s);
    thread_1.stop();
    thread_2.stop();
}


int main()
{
    threads_test();
    return 0;
}

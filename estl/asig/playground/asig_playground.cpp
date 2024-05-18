#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <mutex>
#include <unordered_map>
#include <typeinfo>
#include <ranges>

#include <asig/asig.h>
#include <asig/commands.h>
#include <asig/command_queue.h>

// Some basic explanation
// Given two threads A and B and a commands class instance called cc (command center)
// the 3 forms work like this
// - call()           : From B thread:
//                      cc.call(&Thread_A::member_function, &thread_a, 12);
//                      Will simply call the given function in the receiving thread A's context.
//                      No callback or responses are triggered

// - call_callback()  : From B thread:
//                      cc.call_callback<int>(&Thread_B::callback_squared, this, &Thread_A::square_me, &thread_a, 12);
//                      Will call the given function in the receiving thread A's context
//                      and immediately call the callback function with the returned result.
//                      So, the callback happen in the same thread (A' context) as the command itself is executed!
//                      So in the above example the Thread_A::square_me() function and the callback that gets
//                      squared number as result Thread_B::callback_squared(int squared_val) will happen
//                      in thread A's context.

// - call_response()  : From B thread:
//                      cc.call_response<int>(&Thread_B::callback_squared, this, &Thread_A::square_me, &thread_a, 12);
//                      Will call the given function in the receiving thread A's context
//                      and then push the response call back to thread B's context. This is the typical context at least.
//                      In general the response will be executed in the context in which the object that holds the
//                      Thread_B::callback_squared() member function exists in. Which means from the thread context
//                      that class object has originally called cc.register_command_receiver(this);.

//                      This way you get a completely asynchronous call another thread A's context,
//                      while getting the response back in the calling thread B's context.

//                      Remark: In case you use a lambda/st::function for the response function it will
//                      allways use the the calling thread B's context for the response as there is no
//                      class object registered that that we can lookup.


using namespace std;
using namespace std::ranges;
using namespace lue::asig;
using namespace std::chrono;
using namespace std::chrono_literals;

commands command_center{64};
command_queue queue1{256};

// ------------------------------------------
// --- A quick "hack" to name the threads ---
// ------------------------------------------
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

// ----------------
// --- Thread A ---
// ----------------
struct Thread_A
{
    explicit Thread_A(commands& cc, std::string name) : command_center_(cc), name_ (std::move(name))  {}
    ~Thread_A()  {
        is_running_ = false;
        thread_->join();
    }

    void start  () {
        is_running_ = false;
        thread_ = std::make_unique<std::thread>(&Thread_A::thread_function, this);
    }

    std::thread::id     thread_id      () const { return thread_->get_id(); }
    void                stop           ()       { is_running_ = false;      }
    bool                is_running     () const { return is_running_;       }

    // -------------------------
    // --- Command functions ---
    // -------------------------

    void timer_expired(std::chrono::milliseconds ms_time_out) {
        cerr << "{" << thread_name() << "} " << name_ << "::timer_expired(" << ms_time_out.count() << " ms)\n";
    }

    void mem_fun_no_params() {
        cerr << "{" << thread_name() << "} " << name_ << "::mem_fun_no_params()\n";
    }

    void member_function(int some_number) {
        cerr << "{" << thread_name() << "} " << name_ << "::member_function(" << some_number << ")\n";
    }

    int square_me(const int& some_number) {
        const auto squared = some_number*some_number;
        cerr  << "{" << thread_name() << "} " << name_ << "::square_me(" << some_number << ") => " << squared << "\n";
        return squared;
    }

    // -----------------------------
    // --- Thread main functions ---
    // -----------------------------
    void thread_function()
    {
        command_center_.register_command_receiver(this);
        start_time_ = steady_clock::now();
        add_thread_name("ctx A");
        cerr << "Starting thread {" << thread_name() << "}\n";

        is_running_ = true;
        const auto end_time = start_time_ + 6s;
        while(is_running_ && (steady_clock::now() < end_time) ) {
            std::this_thread::sleep_for(1ms);
            // cerr << "{" << thread_name() << "}  In '" << name_ << "'  Processing commands\n";
            command_center_.execute_all_for_this_thread();
        }
    }

    // ------------------------
    // --- Member variables ---
    // ------------------------
    commands&                       command_center_;
    std::string                     name_               {};
    std::atomic<bool>               is_running_         {false};
    std::unique_ptr<std::thread>    thread_             {nullptr};
    steady_clock::time_point        start_time_{};
};

Thread_A thread_a{command_center, "Thread A"};

// ----------------
// --- Thread B ---
// ----------------
struct Thread_B
{
    explicit Thread_B(commands& cc, std::string name) : command_center_(cc), name_ (std::move(name))  {}
    ~Thread_B()  {
        is_running_ = false;
        thread_->join();
    }

    void start  () {
        is_running_ = false;
        thread_ = std::make_unique<std::thread>(&Thread_B::thread_function, this);
    }

    std::thread::id     thread_id      () const { return thread_->get_id(); }
    void                stop           ()       { is_running_ = false;      }
    bool                is_running     () const { return is_running_;       }

    // -----------------
    // --- Callbacks ---
    // -----------------
    void callback_squared(int squared_number) {
        cerr << "{" << thread_name() << "} " << name_ << "::callback_squared(" << squared_number << ")\n";
    }

    void callback_squared_seq_num(int squared_number, int32_t cmd_seq_num) {
        cerr << "{" << thread_name() << "} " << name_ << "::callback_squared_seq_num(" << squared_number << ") , cmd_seq_num: " << cmd_seq_num << "\n";
    }

    // -----------------------------
    // --- Thread main functions ---
    // -----------------------------
    void thread_function()
    {
        command_center_.register_command_receiver(this);
        add_thread_name("ctx B");
        start_time_ = steady_clock::now();
        std::this_thread::sleep_for(2ms);
        bool can_run_test_commands = true;
        cerr << "Starting thread {" << thread_name() << "}\n";

        is_running_ = true;
        const auto end_time = start_time_ + 6s;
        while(is_running_ && (steady_clock::now() < end_time) ) {
            std::this_thread::sleep_for(1ms);
            // cerr << "{" << thread_name() << "}  In '" << name_ << "'  Processing commands\n";
            command_center_.execute_all_for_this_thread();

            if (can_run_test_commands && (steady_clock::now() > start_time_ + 1s)) {
                can_run_test_commands = false;
                for (int32_t seq_num : views::iota(1, 5)) {
                    test_commands(10*seq_num, seq_num);
                }
            }
        }
        cerr << "{" << thread_name() << "}  EXIT!!\n";
    }

    void test_commands(int square_me_param, int32_t seq_num)
    {
        cerr << "!!!! {" << thread_name() << "} From " << name_ << " call_response()/_callback() Thread_A::square_me(" << square_me_param << ") , cmd_seq_num: " << seq_num << "\n";
        // ---------------------------------------------
        // --- Normal callback and response examples ---
        // ---------------------------------------------
        // command_center_.call_callback<int>(&Thread_B::callback_squared, this, &Thread_A::square_me, &thread_a, square_me_param);
        // command_center_.call_response<int>(&Thread_B::callback_squared, this, &Thread_A::square_me, &thread_a, square_me_param);

        // ---------------------------------------------------
        // --- Using sequence numbers in callback/response ---
        // ---------------------------------------------------
        // command_center_.call_callback<int>(&Thread_B::callback_squared_seq_num, this, seq_num, &Thread_A::square_me, &thread_a, square_me_param);
        command_center_.call_response<int>(&Thread_B::callback_squared_seq_num, this, seq_num, &Thread_A::square_me, &thread_a, square_me_param);

        // ------------------------------------------------------------------
        // --- Lambda callback/response (with sequence number in capture) ---
        // ------------------------------------------------------------------
        // auto lambda_callback_squared = [=](const int& squared_number)  {
        //    cerr << "{" << thread_name() << "} lambda_callback_squared(" << squared_number << "), seq number: " << seq_num << "\n";
        // };
        // command_center.call_callback<int>(lambda_callback_squared, &Thread_A::square_me, &thread_a, square_me_param);
        // command_center.call_response<int>(lambda_callback_squared, &Thread_A::square_me, &thread_a, square_me_param);
    }

    // ------------------------
    // --- Member variables ---
    // ------------------------
    commands&                       command_center_;
    std::string                     name_               {};
    std::atomic<bool>               is_running_         {false};
    std::unique_ptr<std::thread>    thread_             {nullptr};
    steady_clock::time_point        start_time_{};
};


Thread_B thread_b{command_center, "Thread B"};


void threads_test()
{

    thread_a.start();
    thread_b.start();
    while (!(thread_a.is_running() && thread_b.is_running())) {
        this_thread::sleep_for(1ms);
    }

    // ----------------------------------------------------------------------------------------
    // --- Various examples calling from a 3rd thread (main thread) ()comment in as desired ---
    // ----------------------------------------------------------------------------------------
    // Note: All the below examples are NOT using the call_response() since that requires an active "event loop"
    // like we have in threads 1 and 2. We have not created this for the main thread in this example.

    command_center.timer_call_in(300ms, &Thread_A::timer_expired, &thread_a, 300ms);
    command_center.timer_call_in(2s, &Thread_A::timer_expired, &thread_a, 2s);
    command_center.timer_call_in(3s, &Thread_B::test_commands, &thread_b, 12, 5);

    command_center.call(&Thread_A::mem_fun_no_params, &thread_a);
    command_center.call(&Thread_A::member_function, &thread_a, 12);
    int32_t cmd_seq_num = 1;
    command_center.call_callback<int>(&Thread_B::callback_squared_seq_num, &thread_b, cmd_seq_num, &Thread_A::square_me, &thread_a, 7);
    command_center.call_callback<int>(&Thread_B::callback_squared, &thread_b, &Thread_A::square_me, &thread_a, 8);

    auto lambda_callback_squared = [=](int squared_number)  {
        cerr << "{" << thread_name() << "} lambda_callback_squared(" << squared_number << ")\n";
    };
    command_center.call_callback<int>(lambda_callback_squared, &Thread_A::square_me, &thread_a, 8);

    cerr << "\n--- commands playground, Main thread ID: " << thread_name() << "---\n";
    cerr << "command_center.queues_size()       : "  << command_center.command_queues_size() << "\n";
    cerr << "command_center.queues_count()      : "  << command_center.command_queues_count() << "\n";
    cerr << "command_center.receivers_count()   : "  << command_center.receivers_count() << "\n";

    std::this_thread::sleep_for(6s);
    thread_a.stop();
    thread_b.stop();
}


int main()
{
    threads_test();
    return 0;
}

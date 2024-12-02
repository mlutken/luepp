#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <chrono>
#include <mutex>
#include <unordered_map>
#include <typeinfo>

#include <asig/asig.h>

using namespace std;
using namespace lue::asig;
using namespace std::chrono;
using namespace std::chrono_literals;

asig signals;   // Global signals center (This is just a demo after all)

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

// --------------------------------------------------
// --- Events for publish/subscribe demonstration ---
// --------------------------------------------------

struct my_event_t {
    std::string     msg      {};
    float           val      = 0;
    std::string     to_string () const { return "m_event_t: " + msg + ", val: " + std::to_string(val); }
};

// Just to show regitering and calling another class instance
// which we will put asa member in thread A
struct advanced_math
{
  int qube_me(const int& some_number) {
    const auto qubed = std::pow(some_number,3);
    cerr  << "{" << thread_name() << "} MemberThread_A::qube_me(" << some_number << ") => " << qubed << "\n";
    return qubed;
  }
};

// ----------------
// --- Thread A ---
// ----------------
struct Thread_A
{
    explicit Thread_A(asig& sigs, std::string name) : signals_(sigs), name_ (std::move(name))  {}
    ~Thread_A()  {
        is_running_ = false;
        thread_->join();
    }

    void start  () {
        is_running_ = false;
        thread_ = std::make_unique<std::thread>(&Thread_A::thread_function, this);
    }

           // -----------------------------
    // --- Thread main functions ---
    // -----------------------------
    void thread_function()
    {
        add_thread_name("ctx A");
        signals_.register_receiver(this);
        signals_.register_receiver(&advanced_math_);
        signals_.subscribe_permanent<my_event_t>(&Thread_A::my_event_handler, this);

        start_time_ = steady_clock::now();
        cerr << "Starting thread {" << thread_name() << "}\n";

        is_running_ = true;
        const auto end_time = start_time_ + 6s;
        while(is_running_ && (steady_clock::now() < end_time) ) {
            std::this_thread::sleep_for(1ms);
            // cerr << "{" << thread_name() << "}  In '" << name_ << "'  Processing commands\n";
            signals_.execute_all_for_this_thread();
        }
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

    int square_me(const int& some_number) {
        const auto squared = some_number*some_number;
        cerr  << "{" << thread_name() << "} " << name_ << "::square_me(" << some_number << ") => " << squared << "\n";
        return squared;
    }

    // -------------------------------
    // --- Event Handler functions ---
    // -------------------------------
    void my_event_handler(const my_event_t& e)  {
        cerr << "{" << thread_name() << "} " << name_ << "::my_event_t{" <<  e.to_string() << "}\n";
    }

    // ------------------------
    // --- Member variables ---
    // ------------------------
    asig&                           signals_;
    std::string                     name_               {};
    std::atomic<bool>               is_running_         {false};
    std::unique_ptr<std::thread>    thread_             {nullptr};
    steady_clock::time_point        start_time_         {};
    advanced_math                   advanced_math_;
};

Thread_A thread_a{signals, "Thread A"};

// ----------------
// --- Thread B ---
// ----------------
struct Thread_B
{
    explicit Thread_B(asig& sigs, std::string name) : signals_(sigs), name_ (std::move(name))  {}
    ~Thread_B()  {
        is_running_ = false;
        thread_->join();
    }

    void start  () {
        is_running_ = false;
        thread_ = std::make_unique<std::thread>(&Thread_B::thread_function, this);
    }

    // -----------------------------
    // --- Thread main functions ---
    // -----------------------------
    void thread_function()
    {
        add_thread_name("ctx B");
        signals_.register_receiver(this);
        my_event_subscription_ = signals_.subscribe_to<my_event_t>(&Thread_B::my_event_handler, this);

        start_time_ = steady_clock::now();
        std::this_thread::sleep_for(2ms);
        cerr << "Starting thread {" << thread_name() << "}\n";

        is_running_ = true;
        const auto end_time = start_time_ + 6s;
        while(is_running_ && (steady_clock::now() < end_time) ) {
            std::this_thread::sleep_for(1ms);
            signals_.execute_all_for_this_thread();
        }
    }

    std::thread::id     thread_id      () const { return thread_->get_id(); }
    void                stop           ()       { is_running_ = false;      }
    bool                is_running     () const { return is_running_;       }

    // -------------------------
    // --- Command functions ---
    // -------------------------

    void trigger_squared_calculation(int number_to_square, int32_t seq_num)
    {
        cerr << "{" << thread_name() << "} FROM " << name_ << " -> CALL Thread_A::square_me(" << number_to_square << ") , cmd_seq_num: " << seq_num << "\n";
        signals_.call_response<int>(&Thread_B::callback_squared_seq_num, this, seq_num, &Thread_A::square_me, &thread_a, number_to_square);
    }

    // -----------------
    // --- Callbacks ---
    // -----------------
    void callback_squared_seq_num(int squared_number, int32_t cmd_seq_num) {
        cerr << "{" << thread_name() << "} " << name_ << "::callback_squared_seq_num(" << squared_number << ") , cmd_seq_num: " << cmd_seq_num << "\n";
    }

    // -------------------------------
    // --- Event Handler functions ---
    // -------------------------------
    void my_event_handler(const my_event_t& e)  {
        std::this_thread::sleep_for(1ms); // Just to avoid printouts to interleave!
        cerr << "{" << thread_name() << "} " << name_ << "::my_event_t{" <<  e.to_string() << "}\n";
    }


    // ------------------------
    // --- Member variables ---
    // ------------------------
    asig&                           signals_;
    std::string                     name_       {};
    std::atomic<bool>               is_running_ {false};
    std::unique_ptr<std::thread>    thread_     {nullptr};
    steady_clock::time_point        start_time_ {};
    event_subscription              my_event_subscription_;
};


Thread_B thread_b{signals, "Thread B"};


void threads_test()
{
    // Start threads and wait for them to be running and ready!
    thread_a.start();
    thread_b.start();
    while (!(thread_a.is_running() && thread_b.is_running())) {
        this_thread::sleep_for(1ms);
    }

    // Now we can start communicating....
    signals.call(&advanced_math::qube_me, &thread_a.advanced_math_, 5);
    signals.timer_call_in(2s, &Thread_A::timer_expired, &thread_a, 2s);

    // ----------------------------------------------
    // Call function in B after some time out.
    // This function then demonstrates call/response:
    // B -> calls square_me function in A's context.
    // A -> B When square_me is done a new command is send back as response in B's
    //        context with the squared result.
    // ----------------------------------------------------------------------------------------
    signals.timer_call_in(3s, &Thread_B::trigger_squared_calculation, &thread_b, 12, 2);

    // --- Test publish events ---
    std::this_thread::sleep_for(500ms);
    auto evt = my_event_t{"Hello", 12};
    // std::cerr << "---publish(): " << typeid(my_event_t).name() << " " << evt.to_string() << " --- \n";
    signals.publish_event(evt);


    std::this_thread::sleep_for(6s);
    thread_a.stop();
    thread_b.stop();
}


int main()
{
    threads_test();
    return 0;
}

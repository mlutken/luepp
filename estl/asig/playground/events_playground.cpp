#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <typeinfo>

#include <asig/commands.h>
#include <asig/events.h>
#include <asig/command_queue.h>

using namespace std;
using namespace estl;
using namespace std::chrono;
using namespace std::chrono_literals;

events event_center{64};
command_queue queue1{256};

struct my_event_t {
    std::string     msg     {};
    float           val      = 0;
    std::string     to_string () const { return "Msg: " + msg + ", val: " + std::to_string(val); }
};

struct cool_event_t {
    int p1;
    float p2;
};

namespace myspace {
struct cool_event_t {
    int p1;
    float p2;
};
} // namespace myspace

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
    explicit Thread1Class(events& cc, std::string name) : event_center_(cc), name_ (std::move(name))  {}
    ~Thread1Class()  { is_running_ = false; }

    void start  () {
        is_running_ = false;
        thread_ = std::make_unique<std::thread>(&Thread1Class::thread_function, this);
        cerr << "Starting thread ' " << name_ << "' ID: " << thread_->get_id() << "\n";
        thread_->detach();
    }
    void stop           () { is_running_ = false; }
    bool is_running     () const { return is_running_; }

    // -------------------------------
    // --- Event Handler functions ---
    // -------------------------------

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
        auto handler1 = std::function<void (const my_event_t&)>([](const my_event_t& e) { cerr << "Thread 1; Subscriber 1: HANDLE::my_event_t{" <<  e.to_string() << "}\n";} );
        my_event_subscription_1_ = event_center_.subscribe_to_event(std::move(handler1));
        auto handler2 = std::function<void (const my_event_t&)>([](const my_event_t& e) { cerr << "Thread 1; Subscriber 2: HANDLE::my_event_t{" <<  e.to_string() << "}\n";} );
        my_event_subscription_2_ = event_center_.subscribe_to_event(std::move(handler2));

        is_running_ = true;
        const auto end_time = steady_clock::now() + 6s;
        while(is_running_ && (steady_clock::now() < end_time) ) {
            std::this_thread::sleep_for(900ms);
            // cerr << "{" << this_thread::get_id() << "}  In '" << name_ << "'  Processing events\n";
            event_center_.execute_all_for_this_thread();

            idle_work_function();
        }
    }

    void idle_work_function()
    {
    }


    // ------------------------
    // --- Member variables ---
    // ------------------------
    events&                         event_center_;
    std::string                     name_               {};
//    std::shared_ptr<command_queue>  command_queue_      {nullptr};
    std::atomic<bool>               is_running_         {false};
    std::unique_ptr<std::thread>    thread_             {nullptr};
    event_subscription              my_event_subscription_1_;
    event_subscription              my_event_subscription_2_;
};

Thread1Class thread_1_class{event_center, "Thread 1 Class"};

struct Thread2Class
{
    explicit Thread2Class(events& cc, std::string name) : event_center_(cc), name_ (std::move(name))  {}
    ~Thread2Class()  { is_running_ = false; }

    void start  () {
        is_running_ = false;
        thread_ = std::make_unique<std::thread>(&Thread2Class::thread_function, this);
        cerr << "Starting thread ' " << name_ << "' ID: " << thread_->get_id() << "\n";
        thread_->detach();
    }
    void stop           () { is_running_ = false; }
    bool is_running     () const { return is_running_; }

private:

    // -----------------------------
    // --- Thread main functions ---
    // -----------------------------
    void thread_function()
    {
        auto handler1 = std::function<void (const my_event_t&)>([](const my_event_t& e) { cerr << "Thread 2; Subscriber 1: HANDLE::my_event_t{" <<  e.to_string() << "}\n";} );
        my_event_subscription_1_ = event_center_.subscribe_to_event(std::move(handler1));

        is_running_ = true;
        const auto end_time = steady_clock::now() + 6s;
        while(is_running_ && (steady_clock::now() < end_time) ) {
            std::this_thread::sleep_for(10ms);
            // cerr << "{" << this_thread::get_id() << "}  In '" << name_ << "'  Processing events\n";
            event_center_.execute_all_for_this_thread();

            idle_work_function();
        }
    }

    void idle_work_function()
    {
    }


    // ------------------------
    // --- Member variables ---
    // ------------------------
    events&                         event_center_;
    std::string                     name_               {};
    std::atomic<bool>               is_running_         {false};
    std::unique_ptr<std::thread>    thread_             {nullptr};
    event_subscription              my_event_subscription_1_;
};


Thread2Class thread_2_class{event_center, "Thread 2 Class"};



void threads_test()
{

    thread_1_class.start();
    thread_2_class.start();
    while (!(thread_1_class.is_running() && thread_2_class.is_running())) {
        this_thread::sleep_for(1ms);
    }

    this_thread::sleep_for(1s);
    event_center.publish_event<my_event_t>(my_event_t{"Hello", 12});
//    command_center.dbg_print_command_receivers();

    // std::type_info& info = typeid(MyEvent);

//    cerr << "typeid(MyEvent).name         : " << typeid(CoolEvent).name() << "\n";
//    cerr << "typeid(MyEvent).hash         : " << typeid(CoolEvent).hash_code() << "\n";
//    cerr << "typeid(myspace::MyEvent).name: " << typeid(myspace::CoolEvent).name() << "\n";
//    cerr << "typeid(myspace::MyEvent).hash: " << typeid(myspace::CoolEvent).hash_code() << "\n";

//    cerr << "--- commands playground, Main thread ID: " << this_thread::get_id() << "---\n";
//    cerr << "command_center.queues_size()       : "  << command_center.queues_size() << "\n";
//    cerr << "command_center.queues_count()      : "  << command_center.queues_count() << "\n";
//    cerr << "command_center.receivers_count()   : "  << command_center.receivers_count() << "\n";

//    cerr << "thread_1_class       : "  << &thread_1_class  << "\n";
//    cerr << "thread_2_class       : "  << &thread_2_class  << "\n";


    std::this_thread::sleep_for(4s);
    thread_1_class.stop();
    thread_2_class.stop();
}


int main()
{
    threads_test();
    return 0;
}

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


struct TestUnsubscribeOnDeletion
{
    explicit TestUnsubscribeOnDeletion(events& events_center) {
        std::cerr << "TestUnsubscribeOnDeletion::CONSTRUCTOR(): Subscribe!\n";
        subscription_ = events_center.subscribe_to<my_event_t>(&TestUnsubscribeOnDeletion::event_handler, this);
    }

    ~TestUnsubscribeOnDeletion() {
        std::cerr << "TestUnsubscribeOnDeletion::DESTRUCTOR(): Un-subscribe!!! \n";
    }

    void event_handler(const my_event_t& e)  {
        cerr << "Thread 2; TestUnsubscribeOnDeletion::event_handler {" <<  e.to_string() << "} \n";
    }

    event_subscription subscription_;

};

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



private:
    // -------------------------------
    // --- Event Handler functions ---
    // -------------------------------

    void my_event_handler(const my_event_t& e)  {
        cerr << "Thread 1; Subscriber 2: MEM-HANDLE::my_event_t{" <<  e.to_string() << "}\n";
    }

    // -----------------------------
    // --- Thread main functions ---
    // -----------------------------
    void thread_function()
    {
        auto handler1 = std::function<void (const my_event_t&)>([](const my_event_t& e) { cerr << "Thread 1; Subscriber 1: HANDLE::my_event_t{" <<  e.to_string() << "}\n";} );
        event_center_.subscribe_permanent(std::move(handler1));
        event_center_.subscribe_permanent<my_event_t>(&Thread1Class::my_event_handler, this);

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
    event_subscription              my_event_subscription_2_;
};


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
    void stop               () { is_running_ = false; }
    bool is_running         () const { return is_running_; }
    void delete_test_class  ()  { delete_requested_ = true; }

private:

    // -----------------------------
    // --- Thread main functions ---
    // -----------------------------
    void thread_function()
    {
        test_unsubscribe_on_deletion_ = std::make_unique<TestUnsubscribeOnDeletion>(event_center_);
        my_event_subscription_a_ = event_center_.subscribe_to<my_event_t>(&Thread2Class::my_event_handler, this);
        // my_event_subscription_2_ = event_center_.subscribe_to_event<my_event_t>(&Thread2Class::my_event_handler_const, this);

        is_running_ = true;
        const auto end_time = steady_clock::now() + 6s;
        while(is_running_ && (steady_clock::now() < end_time) ) {
            std::this_thread::sleep_for(20ms);
            // cerr << "{" << this_thread::get_id() << "}  In '" << name_ << "'  Processing events\n";
            event_center_.execute_all_for_this_thread();

            idle_work_function();
        }
    }

    void idle_work_function()
    {
        if (delete_requested_) {
            delete_requested_ = false;
            test_unsubscribe_on_deletion_.reset();
        }
    }


    static void my_event_handler_static(const my_event_t& e)  {
        cerr << "Thread 2; Subscriber 3 'my_event_handler_static': HANDLE::my_event_t{" <<  e.to_string() << "}\n";
    }

    // --------------------------------------
    // --- Thread event handler functions ---
    // --------------------------------------
    void my_event_handler(const my_event_t& e)  {
        cerr << "Thread 2; Subscriber A 'my_event_handler': HANDLE::my_event_t{" <<  e.to_string() << "}\n";
        event_center_.un_subscribe(my_event_subscription_a_);
        my_event_subscription_b_ = event_center_.subscribe_to<my_event_t>(&Thread2Class::my_event_handler_const, this);
    }


    void my_event_handler_const(const my_event_t& e) const {
        cerr << "Thread 2; Subscriber B 'my_event_handler_const': HANDLE::my_event_t{" <<  e.to_string() << "}\n";
    }



    // ------------------------
    // --- Member variables ---
    // ------------------------
    events&                         event_center_;
    std::string                     name_               {};
    std::atomic<bool>               is_running_         {false};
    std::unique_ptr<std::thread>    thread_             {nullptr};
    event_subscription              my_event_subscription_a_;
    event_subscription              my_event_subscription_b_;
    std::unique_ptr<TestUnsubscribeOnDeletion> test_unsubscribe_on_deletion_;
    std::atomic_bool                delete_requested_{false};
};




void threads_test()
{

    Thread1Class thread_1_class{event_center, "Thread 1 Class"};
    Thread2Class thread_2_class{event_center, "Thread 2 Class"};
    // std::cerr << "INFO Subscriber count at start: " << event_center.subscribers_count() << "\n";

    thread_1_class.start();
    thread_2_class.start();
    while (!(thread_1_class.is_running() && thread_2_class.is_running())) {
        this_thread::sleep_for(1ms);
    }

    this_thread::sleep_for(1s);
    // std::cerr << "INFO Subscriber count at 1st publish: " << event_center.subscribers_count() << "\n";

    auto evt = my_event_t{"Hello", 12};
    std::cerr << "---publish(): " << typeid(my_event_t).name() << " " << evt.to_string() << " --- \n";
    event_center.publish_event<my_event_t>(evt);
    thread_2_class.delete_test_class();
    std::this_thread::sleep_for(1s);

    // std::cerr << "INFO Subscriber count at 2nd publish: " << event_center.subscribers_count() << "\n";
    evt = my_event_t{"Hello Again!", 33};
    std::cerr << "---publish(): " << typeid(my_event_t).name() << " " << evt.to_string() << " --- \n";
    event_center.publish_event<my_event_t>(evt);
    std::this_thread::sleep_for(2s);
    thread_1_class.stop();
    thread_2_class.stop();
}


int main()
{
    threads_test();
    return 0;
}

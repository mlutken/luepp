#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <mutex>
#include <unordered_map>
#include <typeinfo>

#include <asig/asig.h>

// using namespace std;
using namespace std::chrono_literals;


// Global signals and pointer to threads (This is just a demo after all)
struct Thread_A;
struct Thread_B;
lue::asig::asig signals;
std::unique_ptr<Thread_A> thread_a = nullptr;
std::unique_ptr<Thread_B> thread_b = nullptr;

// --------------------------------------------------
// --- Events for publish/subscribe demonstration ---
// --------------------------------------------------

struct my_event_t {
  std::string     msg      {};
  float           val      = 0;
  std::string     to_string () const { return "m_event_t: " + msg + ", val: " + std::to_string(val); }
};


// ----------------
// --- Thread A ---
// ----------------
struct Thread_A
{
  Thread_A() {
    thread_ = std::make_unique<std::thread>(&Thread_A::thread_function, this);
  }

  // -----------------------------
  // --- Thread main functions ---
  // -----------------------------
  void thread_function()
  {
    signals.register_receiver(this);
    signals.subscribe_permanent<my_event_t>(&Thread_A::my_event_handler, this);

    is_running_ = true;
    while(is_running_ ) {
      std::this_thread::sleep_for(1ms);
      signals.execute_all_for_this_thread();
    }
  }

  // -------------------------
  // --- Command functions ---
  // -------------------------

  void timer_expired(std::chrono::milliseconds ms_time_out) {
    std::cerr << "Thread_A::timer_expired(" << ms_time_out.count() << " ms)\n";
  }

  void set_engine_speed(int speed) {
    std::cerr  << "Thread_A::set_engine_speed to: " << speed << "\n";
  }

  int square_me(const int& some_number) {
    std::cerr  << "Thread_A::square_me(" << some_number << ") => " << some_number*some_number << "\n";
    return some_number*some_number;
  }

  // -------------------------------
  // --- Event Handler functions ---
  // -------------------------------
  void my_event_handler(const my_event_t& e)  {
    std::cerr << "Thread_A::my_event_t{" <<  e.to_string() << "}\n";
  }

  // ------------------------
  // --- Member variables ---
  // ------------------------
  std::atomic<bool>               is_running_         {false};
  std::unique_ptr<std::thread>    thread_             {nullptr};
};

// ----------------
// --- Thread B ---
// ----------------
struct Thread_B
{
  Thread_B() {
    thread_ = std::make_unique<std::thread>(&Thread_B::thread_function, this);
  }

  ~Thread_B()  {
    is_running_ = false;
    thread_->join();
  }

  // -----------------------------
  // --- Thread main functions ---
  // -----------------------------
  void thread_function()
  {
    signals.register_receiver(this);
    my_event_subscription_ = signals.subscribe_to<my_event_t>(&Thread_B::my_event_handler, this);

    is_running_ = true;
    while(is_running_) {
      std::this_thread::sleep_for(1ms);
      signals.execute_all_for_this_thread();
    }
  }

  // -------------------------
  // --- Command functions ---
  // -------------------------

  void trigger_squared_calculation(int number_to_square)
  {
    std::cerr << "FROM Thread_B -> CALL Thread_A::square_me(" << number_to_square << ")\n";
    signals.call_response<int>(&Thread_B::callback_squared, this, &Thread_A::square_me, thread_a.get(), number_to_square);
  }

  // -----------------
  // --- Callbacks ---
  // -----------------
  void callback_squared(int squared_number) {
    std::cerr << "Thread_B::callback_squared(" << squared_number << ")\n";
  }

  // -------------------------------
  // --- Event Handler functions ---
  // -------------------------------
  void my_event_handler(const my_event_t& e)  {
    std::this_thread::sleep_for(1ms); // Just to avoid printouts to interleave!
    std::cerr << "Thread_B::my_event_t{" <<  e.to_string() << "}\n";
  }


  // ------------------------
  // --- Member variables ---
  // ------------------------
  std::atomic<bool>               is_running_ {false};
  std::unique_ptr<std::thread>    thread_     {nullptr};
  lue::asig::event_subscription   my_event_subscription_;
};

void threads_test()
{
  thread_a = std::make_unique<Thread_A>();
  thread_b = std::make_unique<Thread_B>();

  while (!(thread_a->is_running_ && thread_b->is_running_)) {
    std::this_thread::sleep_for(1ms);
  }

  signals.call(&Thread_A::set_engine_speed, thread_a.get(), 27);

  // ----------------------------------------------
  // Call function in B after some time out.
  // This function then demonstrates call/response:
  // B -> calls square_me function in A's context.
  // A -> B When square_me is done a new command is send back as response in B's
  //        context with the squared result.
  // ----------------------------------------------------------------------------------------
  signals.timer_call_in(2s, &Thread_A::timer_expired, thread_a.get(), 2s);
  signals.timer_call_in(3s, &Thread_B::trigger_squared_calculation, thread_b.get(), 12);

  // --- Test publish events ---
  std::this_thread::sleep_for(500ms);
  auto evt = my_event_t{"Hello", 12};
  std::cerr << "---publish(): " << typeid(my_event_t).name() << " " << evt.to_string() << " --- \n";
  signals.publish_event(evt);


  std::this_thread::sleep_for(6s);
  thread_a->is_running_ = false;
  thread_b->is_running_ = false;
}


int main()
{
  threads_test();
  return 0;
}

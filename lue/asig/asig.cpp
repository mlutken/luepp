#include "asig.h"

#include <iostream>

using namespace std;

namespace lue::asig
{

asig::asig()
    : commands_()
{
}

asig::asig(size_t command_queues_size, size_t timer_queues_size, size_t event_queues_size)
    : commands_(command_queues_size, timer_queues_size),
      events_(event_queues_size)
{
}

void asig::execute_all_for_this_thread()
{
    commands_.execute_all_for_this_thread();
    events_.execute_all_for_this_thread();
}

void asig::register_receiver(void* class_instance_ptr, std::thread::id thread_id)
{
    commands_.register_receiver(class_instance_ptr, thread_id);
}

void asig::un_subscribe(event_subscription& subscription, std::thread::id thread_id)
{
  events_.un_subscribe(subscription, thread_id);
}

} // END namespace lue::asig

#include "events_queue.h"

namespace estl::asig {

events_queue::events_queue(size_t queue_size)
 :  queue_(queue_size),
    push_mutex_()
{

}

std::unique_ptr<events_queue::event_data_base_t> events_queue::pop_front()
{
    if (queue_.empty()) { return nullptr; }
    auto event_data_ptr = std::move(queue_.front());
    queue_.pop();
    return event_data_ptr;
}


} // END namespace estl::asig

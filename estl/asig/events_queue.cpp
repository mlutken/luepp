#include "events_queue.h"

namespace estl {

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

//void events_queue::push(std::function<void ()>&& command_fun)
//{
//    std::scoped_lock<std::mutex> lock(push_mutex_);
//    queue_.push(cmd_t{std::move(command_fun)});
//}

//bool events_queue::execute_next()
//{
//    if (queue_.empty()) {
//        return false;
//    }
//    auto& front = queue_.front();
//    if (front.command_fun) {
//        front.command_fun();
//    }
//    queue_.pop();
//    return true;
//}




} // END namespace estl

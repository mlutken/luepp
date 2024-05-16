#include "command_queue.h"

namespace estl::asig {

command_queue::command_queue(size_t queue_size)
 :  queue_(queue_size),
    push_mutex_()
{

}

void command_queue::execute_all()
{
    while (!empty()) {
        execute_next();
    }
}

bool command_queue::execute_next()
{
    if (queue_.empty()) {
        return false;
    }
    auto& front = queue_.front();
    if (front.command_fun) {
        front.command_fun();
    }
    queue_.pop();
    return true;
}

void command_queue::push(std::function<void ()>&& command_fun)
{
    std::scoped_lock<std::mutex> lock(push_mutex_);
    queue_.push(cmd_t{std::move(command_fun)});
}





} // END namespace estl::asig

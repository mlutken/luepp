#include "command_queue.h"

namespace lue::asig {

command_queue::command_queue(size_t queue_size):
    queue_(queue_size),
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
    if (front) {
        front->execute();
    }
    queue_.pop();
    return true;
}


void command_queue::do_push_command(std::unique_ptr<command_base_t> cmd)
{
    std::scoped_lock<std::mutex> lock(push_mutex_);
    queue_.push(std::move(cmd));
}





} // END namespace lue::asig

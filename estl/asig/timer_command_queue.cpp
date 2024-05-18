#include "timer_command_queue.h"

namespace lue::asig {

timer_command_queue::timer_command_queue(size_t queue_size):
    input_queue_(queue_size),
    input_queue_mutex_()
{

}

void timer_command_queue::execute_all()
{
    process_input_queue();
    bool done = false;
    while (!done) {
        done = !check_front_expired_and_execute();
    }
}




void timer_command_queue::do_push_command(std::unique_ptr<timer_cmd_base_t> cmd)
{
    std::scoped_lock<std::mutex> lock(input_queue_mutex_);
    input_queue_.push(std::move(cmd));
}

/// Return true if a command has expired (and was thus executed)
bool timer_command_queue::check_front_expired_and_execute()
{
    if (pending_timers_.empty()) {
        return false;
    }
    const auto front_it = pending_timers_.begin();
    auto& cmd_ptr = front_it->second;
    if (cmd_ptr->has_expired()) {
        cmd_ptr->execute();
        pending_timers_.erase(front_it);
        return true;
    }
    return false;
}

void timer_command_queue::process_input_queue()
{
    while (!input_queue_.empty()) {
        auto& front = input_queue_.front();
        if (front) {
            put_in_pending_timers(std::move(front));
        }
        input_queue_.pop();
    }
}

/** Insert in pending timers, but first test if timer has already expired in which case simply
    call the associated command/function and omit putting it into pending timers. */
void timer_command_queue::put_in_pending_timers(std::unique_ptr<timer_cmd_base_t> cmd)
{
    if (cmd->has_expired()) {
        cmd->execute();
    }
    else {
        pending_timers_[cmd->call_at_tp_] = std::move(cmd);
    }
}

/** Insert in pending timers, but first test if timer has already expired in which case simply
    call the associated command/function and omit putting it into pending timers. */
void timer_command_queue::put_in_pending_timers(std::unique_ptr<timer_cmd_base_t> cmd, time_point current_time_point)
{
    if (cmd->has_expired(current_time_point)) {
        cmd->execute();
    }
    else {
        pending_timers_[cmd->call_at_tp_] = std::move(cmd);
    }
}





} // END namespace lue::asig

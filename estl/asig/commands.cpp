#include "commands.h"

#include <iostream>

using namespace std;

namespace estl::asig {

commands::commands()
    :   thread_lookup_mutex_()
{

}

commands::commands(size_t command_queues_size)
    :   thread_lookup_mutex_(),
        command_queues_size_(command_queues_size)
{

}

void commands::execute_all_for_this_thread()
{
    const auto& cmd_queue_ptr = get_command_receiver_queue(std::this_thread::get_id());
    if (cmd_queue_ptr) {
        cmd_queue_ptr->execute_all();
    }

    const auto& timer_queue_ptr = get_timer_receiver_queue(std::this_thread::get_id());
    if (timer_queue_ptr) {
        timer_queue_ptr->execute_all();
    }
}


void commands::register_command_receiver(void* class_instance_ptr, std::thread::id thread_id)
{
    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
    cmd_queues_.emplace(thread_id, std::shared_ptr<command_queue>(new command_queue{command_queues_size_}));
    timer_queues_.emplace(thread_id, std::shared_ptr<timer_command_queue>(new timer_command_queue{timer_command_queues_size_}));
    receiver_lookup_.emplace(class_instance_ptr, thread_id);
}


size_t commands::command_queues_count() const
{
    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
    return cmd_queues_.size();
}

size_t commands::receivers_count() const
{
    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
    return receiver_lookup_.size();
}

void commands::dbg_print_command_receivers() const
{
    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
    cerr << "*** Printing all receiver classes, sorted per thread ***\n";
    for (auto& [print_thread_id, queue_ptr]: cmd_queues_) {
        cerr << "[thread id: " << print_thread_id << "]\n";

        for (auto& [class_ptr, look_fo_thread_id] : receiver_lookup_) {
            if (print_thread_id == look_fo_thread_id) {
                cerr << "   Class: " << class_ptr << "\n";
            }
        }
    }
}

commands::command_queue_ptr_t commands::get_command_receiver_queue(std::thread::id thread_id)
{
    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
    const auto it_queue = cmd_queues_.find(thread_id);
    if (it_queue != cmd_queues_.end()) {
        return it_queue->second;
    }
    return commands::command_queue_ptr_t(nullptr);
}

commands::command_queue_ptr_t commands::get_command_receiver_queue(void* class_instance_ptr)
{
    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
    const auto it_rec = receiver_lookup_.find(class_instance_ptr);
    if (it_rec != receiver_lookup_.end()) {
        const auto thread_id = it_rec->second;
        const auto it_queue = cmd_queues_.find(thread_id);
        if (it_queue != cmd_queues_.end()) {
            return it_queue->second;
        }
    }
    return commands::command_queue_ptr_t(nullptr);
}

commands::timer_queue_ptr_t commands::get_timer_receiver_queue(std::thread::id thread_id)
{
    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
    const auto it_queue = timer_queues_.find(thread_id);
    if (it_queue != timer_queues_.end()) {
        return it_queue->second;
    }
    return commands::timer_queue_ptr_t(nullptr);
}

commands::timer_queue_ptr_t commands::get_timer_receiver_queue(void* class_instance_ptr)
{
    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
    const auto it_rec = receiver_lookup_.find(class_instance_ptr);
    if (it_rec != receiver_lookup_.end()) {
        const auto thread_id = it_rec->second;
        const auto it_queue = timer_queues_.find(thread_id);
        if (it_queue != timer_queues_.end()) {
            return it_queue->second;
        }
    }
    return commands::timer_queue_ptr_t(nullptr);
}

} // END namespace estl::asig

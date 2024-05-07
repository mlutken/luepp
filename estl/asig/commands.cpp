#include "commands.h"

namespace estl {

commands::commands()
    :   thread_lookup_mutex_()
{

}

commands::commands(size_t command_queues_size)
    :   thread_lookup_mutex_(),
        command_queues_size_(command_queues_size)
{

}



commands::queue_ptr_t commands::register_receiver(void* class_instance_ptr, std::thread::id thread_id)
{
    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);

    if (!cmd_queues_.contains(thread_id)) {
        // Create command queue for this thread
        cmd_queues_.emplace(thread_id, std::shared_ptr<command_queue>(new command_queue{command_queues_size_}));
    }
    receiver_lookup_[class_instance_ptr] = thread_id;
    return cmd_queues_[thread_id];
}

commands::queue_ptr_t commands::get_receiver_queue(std::thread::id thread_id)
{
    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
    const auto it_queue = cmd_queues_.find(thread_id);
    if (it_queue != cmd_queues_.end()) {
        return it_queue->second;
    }
    return commands::queue_ptr_t(nullptr);
}

commands::queue_ptr_t commands::get_receiver_queue(void* class_instance_ptr)
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
    return commands::queue_ptr_t(nullptr);
}

size_t commands::queues_count() const
{
    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
    return cmd_queues_.size();
}

size_t commands::receivers_count() const
{
    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
    return receiver_lookup_.size();
}



} // END namespace estl

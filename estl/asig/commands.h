#pragma once

#include <thread>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <functional>

#include <asig/command_queue.h>

// https://en.cppreference.com/w/cpp/utility/functional/invoke

namespace estl {

class commands
{
public:
    using queue_ptr_t = std::shared_ptr<command_queue>;

    commands    ();
    explicit    commands    (size_t command_queues_size);

    void        context_call        ();

    queue_ptr_t register_receiver   (void* class_instance_ptr, std::thread::id thread_id = std::this_thread::get_id());
    queue_ptr_t get_receiver_queue  (std::thread::id thread_id = std::this_thread::get_id());
    queue_ptr_t get_receiver_queue  (void* class_instance_ptr);

    size_t      queues_count        () const;
    size_t      receivers_count     () const;
    size_t      queues_size         () const { return command_queues_size_; }

private:
    using cmd_queues_map_t      = std::unordered_map<std::thread::id, std::shared_ptr<command_queue>>;
    using receiver_lookup_map_t = std::unordered_map<void*, std::thread::id>;
    mutable std::mutex      thread_lookup_mutex_;
    size_t                  command_queues_size_ = 128;
    cmd_queues_map_t        cmd_queues_;
    receiver_lookup_map_t   receiver_lookup_;



};


} // END namespace estl

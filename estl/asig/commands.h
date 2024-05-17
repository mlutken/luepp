#pragma once

#include <thread>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <functional>

#include <asig/command_queue.h>

// https://en.cppreference.com/w/cpp/utility/functional/invoke

namespace estl::asig {


class commands
{
public:
    using queue_ptr_t = std::shared_ptr<command_queue>;

    commands    ();
    explicit    commands (size_t command_queues_size);
    void        execute_all_for_this_thread ();


    template<class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call(const CommandMemberCallable& command_member_fun,
              CommandClassObject* command_class_obj_ptr,
              const CommandArgs&... args)
    {
        auto cmd_queue_ptr = get_receiver_queue(command_class_obj_ptr);
        if (!cmd_queue_ptr) { return; }
        command_queue& cmd_queue = *cmd_queue_ptr;
        cmd_queue.call<CommandMemberCallable, CommandClassObject, CommandArgs...>
            (command_member_fun, command_class_obj_ptr, args...);
    }

    template<class ReturnType,
             class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call_callback(const std::function<void (const ReturnType&)>& callback_function,
                       const CommandMemberCallable& command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       const CommandArgs&... command_args
                       )
    {
        auto cmd_queue_ptr = get_receiver_queue(command_class_obj_ptr);
        if (!cmd_queue_ptr) { return; }
        command_queue& cmd_queue = *cmd_queue_ptr;

        cmd_queue.call_callback<ReturnType, CommandMemberCallable, CommandClassObject, CommandArgs...>
            (callback_function, command_member_fun, command_class_obj_ptr, command_args...);
    }

    template<class ReturnType,
             class CallbackMemberCallable,
             class CallbackClassObject,
             class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call_callback(const CallbackMemberCallable& callback_member_fun,
                       CallbackClassObject* callback_class_obj_ptr,
                       const CommandMemberCallable& command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       const CommandArgs&... command_args
                       )
    {
        auto cmd_queue_ptr = get_receiver_queue(command_class_obj_ptr);
        if (!cmd_queue_ptr) { return; }
        command_queue& cmd_queue = *cmd_queue_ptr;

        cmd_queue.call_callback<ReturnType, CallbackMemberCallable, CallbackClassObject, CommandMemberCallable, CommandClassObject, CommandArgs...>
            (callback_member_fun, callback_class_obj_ptr, command_member_fun, command_class_obj_ptr, command_args...);
    }

    template<class ReturnType,
             class CallbackMemberCallable,
             class CallbackClassObject,
             class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call_callback(const CallbackMemberCallable& callback_member_fun,
                       CallbackClassObject* callback_class_obj_ptr,
                       std::int32_t cmd_seq_num,
                       const CommandMemberCallable& command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       const CommandArgs&... command_args
                       )
    {
        auto cmd_queue_ptr = get_receiver_queue(command_class_obj_ptr);
        if (!cmd_queue_ptr) { return; }
        command_queue& cmd_queue = *cmd_queue_ptr;

        cmd_queue.call_callback<ReturnType, CallbackMemberCallable, CallbackClassObject, CommandMemberCallable, CommandClassObject, CommandArgs...>
            (callback_member_fun, callback_class_obj_ptr, cmd_seq_num, command_member_fun, command_class_obj_ptr, command_args...);
    }

    template<class ReturnType,
             class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call_response(const std::function<void (const ReturnType&)>& response_function,
                       const CommandMemberCallable& command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       const CommandArgs&... command_args
                       )
    {
        auto cmd_queue_ptr = get_receiver_queue(command_class_obj_ptr);
        if (!cmd_queue_ptr) { return; }
        auto resp_queue_ptr = get_receiver_queue(std::this_thread::get_id());
        if (!resp_queue_ptr) { return; }
        command_queue& cmd_queue = *cmd_queue_ptr;

        cmd_queue.call_response<ReturnType, CommandMemberCallable, CommandClassObject, CommandArgs...>
            (*resp_queue_ptr, response_function, command_member_fun, command_class_obj_ptr, command_args...);
    }


    template<class ReturnType,
             class ResponseMemberCallable,
             class ResponseClassObject,
             class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call_response(const ResponseMemberCallable& response_member_fun,
                       ResponseClassObject* response_class_obj_ptr,
                       const CommandMemberCallable& command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       const CommandArgs&... command_args
                       )
    {
        auto cmd_queue_ptr = get_receiver_queue(command_class_obj_ptr);
        if (!cmd_queue_ptr) { return; }
        auto resp_queue_ptr = get_receiver_queue(response_class_obj_ptr);
        if (!resp_queue_ptr) { return; }
        command_queue& cmd_queue = *cmd_queue_ptr;

        cmd_queue.call_response<ReturnType, ResponseMemberCallable, ResponseClassObject, CommandMemberCallable, CommandClassObject, CommandArgs...>
            (*resp_queue_ptr, response_member_fun, response_class_obj_ptr, command_member_fun, command_class_obj_ptr, command_args...);
    }

    template<class ReturnType,
             class ResponseMemberCallable,
             class ResponseClassObject,
             class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call_response(const ResponseMemberCallable& response_member_fun,
                       ResponseClassObject* response_class_obj_ptr,
                       std::int32_t cmd_seq_num,
                       const CommandMemberCallable& command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       const CommandArgs&... command_args
                       )
    {
        auto cmd_queue_ptr = get_receiver_queue(command_class_obj_ptr);
        if (!cmd_queue_ptr) { return; }
        auto resp_queue_ptr = get_receiver_queue(response_class_obj_ptr);
        if (!resp_queue_ptr) { return; }
        command_queue& cmd_queue = *cmd_queue_ptr;

        cmd_queue.call_response<ReturnType, ResponseMemberCallable, ResponseClassObject, CommandMemberCallable, CommandClassObject, CommandArgs...>
            (*resp_queue_ptr, response_member_fun, response_class_obj_ptr, cmd_seq_num, command_member_fun, command_class_obj_ptr, command_args...);
    }



    void        register_command_receiver   (void* class_instance_ptr, std::thread::id thread_id = std::this_thread::get_id());
    queue_ptr_t get_receiver_queue          (std::thread::id thread_id = std::this_thread::get_id());
    queue_ptr_t get_receiver_queue          (void* class_instance_ptr);

    size_t      queues_count                () const;
    size_t      receivers_count             () const;
    size_t      queues_size                 () const { return command_queues_size_; }

    void        dbg_print_command_receivers () const;

private:
    using cmd_queues_map_t          = std::unordered_map<std::thread::id, std::shared_ptr<command_queue>>;

    using receiver_lookup_map_t     = std::unordered_map<void*, std::thread::id>;

    mutable std::mutex      thread_lookup_mutex_;
    size_t                  command_queues_size_ = 128;
    cmd_queues_map_t        cmd_queues_;
    receiver_lookup_map_t   receiver_lookup_;
};


} // END namespace estl::asig

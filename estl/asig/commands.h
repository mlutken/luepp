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

    template<class CommandCallable,
             class CommandClassObject,
             typename ... CommandArgs >
    void send ( CommandCallable command_fun,
                CommandClassObject* command_class_obj,
                CommandArgs... command_args
                )
    {
        std::cerr << "Member function callback: command_class_instance: " << command_class_obj << "\n";
        auto cmd_queue = get_receiver_queue(command_class_obj);
        if (!cmd_queue) {
            return;
        }
        auto cmd = [=]() {
            return std::invoke(command_fun, command_class_obj, command_args...);
        };

        command_queue& cc = *cmd_queue;
        cc.push(std::move(cmd));
    }

    template<class ReturnType,
             class ResultMemberCallable,
             class ResponseClassObject,
             class CommandCallable,
             class CommandClassObject,
             typename ... CommandArgs >
    void send_response(ResultMemberCallable response_member_fun,
                       ResponseClassObject* response_class_obj_ptr,
                       std::int32_t cmd_seq_num,
                       CommandCallable command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       CommandArgs... command_args
                       )
    {
        auto cmd_queue_ptr = get_receiver_queue(command_class_obj_ptr);
        if (!cmd_queue_ptr) {
            return;
        }
        auto cb_queue = get_receiver_queue(response_class_obj_ptr);
        auto cmd = [=]() -> ReturnType {
            return std::invoke(command_member_fun, command_class_obj_ptr, command_args...);
        };


        auto cb = [=](const ReturnType& cmd_return_value){
            return std::invoke(response_member_fun, response_class_obj_ptr, cmd_seq_num, cmd_return_value );
        };
        command_queue& cmd_queue = *cmd_queue_ptr;
        cmd_queue.push_response<ReturnType>(std::move(cmd), std::move(cb), cb_queue);
    }


    template<class ReturnType,
             class ResultMemberCallable,
             class ResponseClassObject,
             class CommandCallable,
             class CommandClassObject,
             typename ... CommandArgs >
    void send_response ( ResultMemberCallable response_member_fun,
                         ResponseClassObject* response_class_obj_ptr,
                         CommandCallable command_member_fun,
                         CommandClassObject* command_class_obj_ptr,
                         CommandArgs... command_args
                    )
    {
        // std::cerr << "send_response : command_class_obj  : " << command_class_obj_ptr << "\n";
        // std::cerr << "send_response : response_class_obj : " << response_class_obj_ptr << "\n";
        auto cmd_queue_ptr = get_receiver_queue(command_class_obj_ptr);
        if (!cmd_queue_ptr) {
            return;
        }
        auto cb_queue = get_receiver_queue(response_class_obj_ptr);
        auto cmd = [=]() -> ReturnType {
            return std::invoke(command_member_fun, command_class_obj_ptr, command_args...);
        };


        auto cb = [=](const ReturnType& cmd_return_value){
            return std::invoke(response_member_fun, response_class_obj_ptr, cmd_return_value );
        };
        command_queue& cmd_queue = *cmd_queue_ptr;
        cmd_queue.push_response<ReturnType>(std::move(cmd), std::move(cb), cb_queue);
    }

    template<typename ReturnType,
             class CallbackCallable,
             class CommandCallable,
             class CommandClassObject,
             typename ... CommandArgs >
    void send_callback( CallbackCallable callback_fun,
                        CommandCallable command_member_fun,
                        CommandClassObject* command_class_obj_ptr,
                        CommandArgs... command_args
                        )
    {
        auto cmd_queue_ptr = get_receiver_queue(command_class_obj_ptr);
        if (!cmd_queue_ptr) {
            return;
        }
        auto command_fn = [=]() -> ReturnType {
            return std::invoke(command_member_fun, command_class_obj_ptr, command_args...);
        };

        auto result_callback_fn = [=](const ReturnType& cmd_return_value){
            return std::invoke(callback_fun, cmd_return_value );
        };
        command_queue& cmd_queue = *cmd_queue_ptr;
        cmd_queue.push_callback<ReturnType>(std::move(command_fn), std::move(result_callback_fn));
    }

    void        register_receiver   (void* class_instance_ptr, std::thread::id thread_id = std::this_thread::get_id());
    queue_ptr_t get_receiver_queue  (std::thread::id thread_id = std::this_thread::get_id());
    queue_ptr_t get_receiver_queue  (void* class_instance_ptr);

    size_t      queues_count        () const;
    size_t      receivers_count     () const;
    size_t      queues_size         () const { return command_queues_size_; }

    void        dbg_print_receivers () const;

private:
    using cmd_queues_map_t      = std::unordered_map<std::thread::id, std::shared_ptr<command_queue>>;
    using receiver_lookup_map_t = std::unordered_map<void*, std::thread::id>;
    mutable std::mutex      thread_lookup_mutex_;
    size_t                  command_queues_size_ = 128;
    cmd_queues_map_t        cmd_queues_;
    receiver_lookup_map_t   receiver_lookup_;



};


} // END namespace estl

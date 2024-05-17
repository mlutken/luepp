#pragma once

#include <iostream>
#include <mutex>
#include <functional>
#include <memory>
#include <concurrent/srsw_fifo.hpp>

// https://en.cppreference.com/w/cpp/utility/functional/invoke
// https://codereview.stackexchange.com/questions/221828/c17-implementation-of-stdinvoke
// https://stackoverflow.com/questions/46388524/when-to-use-stdinvoke-instead-of-simply-calling-the-invokable
// https://stackoverflow.com/questions/9831501/how-can-i-have-multiple-parameter-packs-in-a-variadic-template

namespace estl::asig {

class command_queue
{
public:
    explicit command_queue  (size_t queue_size = 128);

    void    execute_all     ();
    bool    execute_next    ();


    template<class CommandCallable>
    void push_cmd (const CommandCallable& command_fun)
    {
         do_push_command(command_t<CommandCallable>::create(command_fun));
    }

    template<class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call(const CommandMemberCallable& command_member_fun,
              CommandClassObject* command_class_obj_ptr,
              const CommandArgs&... args)
    {
        auto cmd = [=]() {
            std::invoke(command_member_fun, command_class_obj_ptr, args...);
        };
        push_cmd(std::move(cmd));
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
        auto to_send_fn = [=](){
            auto cmd_return_value = std::invoke(command_member_fun, command_class_obj_ptr, command_args...);
            std::invoke(callback_function, cmd_return_value);
        };
        push_cmd(std::move(to_send_fn));
    }


    template<class ReturnType,
             class CallbackMemberCallable,
             class CallbackClassObject,
             class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call_callback (const CallbackMemberCallable& callback_member_fun,
                       CallbackClassObject* callback_class_obj_ptr,
                       const CommandMemberCallable& command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       const CommandArgs&... command_args
                       )
    {
        auto to_send_fn = [=](){
            auto cmd_return_value = std::invoke(command_member_fun, command_class_obj_ptr, command_args...);
            std::invoke(callback_member_fun, callback_class_obj_ptr, cmd_return_value);
        };
        push_cmd(std::move(to_send_fn));
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
        auto to_send_fn = [=](){
            auto cmd_return_value = std::invoke(command_member_fun, command_class_obj_ptr, command_args...);
            std::invoke(callback_member_fun, callback_class_obj_ptr, cmd_return_value, cmd_seq_num);
        };
        push_cmd(std::move(to_send_fn));
    }

    template<class ReturnType,
             class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call_response(command_queue& response_queue,
                       const std::function<void (const ReturnType&)>& response_function,
                       const CommandMemberCallable& command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       const CommandArgs&... command_args
                       )
    {
        command_queue* response_queue_ptr = &response_queue;

        auto to_send_fn = [=]() {
            auto cmd_return_value = std::invoke(command_member_fun, command_class_obj_ptr, command_args...);
            auto response_cmd_fn = [=]() {
                std::invoke(response_function, cmd_return_value);
            };
            response_queue_ptr->push_cmd(std::move(response_cmd_fn));
        };

        push_cmd(std::move(to_send_fn));
    }

    template<class ReturnType,
             class ResponseMemberCallable,
             class ResponseClassObject,
             class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call_response(command_queue& response_queue,
                       const ResponseMemberCallable& response_member_fun,
                       ResponseClassObject* response_class_obj_ptr,
                       const CommandMemberCallable& command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       const CommandArgs&... command_args
                       )
    {
        command_queue* response_queue_ptr = &response_queue;

        auto to_send_fn = [=]() {
            auto cmd_return_value = std::invoke(command_member_fun, command_class_obj_ptr, command_args...);
            auto response_cmd_fn = [=]() {
                std::invoke(response_member_fun, response_class_obj_ptr, cmd_return_value);
            };
            response_queue_ptr->push_cmd(std::move(response_cmd_fn));
        };

        push_cmd(std::move(to_send_fn));
    }

    template<class ReturnType,
             class ResponseMemberCallable,
             class ResponseClassObject,
             class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call_response(command_queue& response_queue,
                       const ResponseMemberCallable& response_member_fun,
                       ResponseClassObject* response_class_obj_ptr,
                       std::int32_t cmd_seq_num,
                       const CommandMemberCallable& command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       const CommandArgs&... command_args
                       )
    {
        command_queue* response_queue_ptr = &response_queue;

        auto to_send_fn = [=]() {
            auto cmd_return_value = std::invoke(command_member_fun, command_class_obj_ptr, command_args...);
            auto response_cmd_fn = [=]() {
                std::invoke(response_member_fun, response_class_obj_ptr, cmd_return_value, cmd_seq_num);
            };
            response_queue_ptr->push_cmd(std::move(response_cmd_fn));
        };

        push_cmd(std::move(to_send_fn));
    }


    size_t  size            () const { return queue_.size();        }
    size_t  capacity        () const { return queue_.capacity();    }
    bool    empty           () const { return queue_.empty();       }


private:
    struct command_base_t {
        virtual ~command_base_t() = default;
        virtual void execute() = 0;
    };

    template<class Callable>
    struct command_t : public command_base_t {
        explicit command_t(Callable&& fn) : fn_(std::move(fn)) {}

        static std::unique_ptr<command_base_t> create(Callable&& fn) {
            return std::unique_ptr<command_base_t>(new command_t<Callable>(std::move(fn)) );
        }

        explicit command_t(const Callable& fn) : fn_(fn) {}

        static std::unique_ptr<command_base_t> create(const Callable& fn) {
            return std::unique_ptr<command_base_t>(new command_t<Callable>(fn) );
        }

        void execute() override {
            fn_();
        }

        Callable fn_;
    };

    void do_push_command     (std::unique_ptr<command_base_t> cmd);


    using queue_t       = estl::srsw_fifo<std::unique_ptr<command_base_t>>;

    queue_t         queue_;
    std::mutex      push_mutex_;

};

} // END namespace estl::asig


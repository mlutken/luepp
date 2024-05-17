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

struct command_base_t {
    virtual ~command_base_t() = default;
    virtual void execute() {}
    void operator()() {
        execute();
    }
};

template<class Callable>
struct command_t : public command_base_t {
    explicit command_t(Callable&& fn) : fn_(std::forward(fn)) {
//        std::cerr << " ## command_t MOVING constructor\n";
    }

    static std::unique_ptr<command_base_t> create(Callable&& fn) {
        return std::unique_ptr<command_base_t>(new command_t<Callable>(std::move(fn)) );
    }

    explicit command_t(const Callable& fn) : fn_(fn) {
//        std::cerr << " %% command_t CLASSIC constructor\n";
    }

    static std::unique_ptr<command_base_t> create(const Callable& fn) {
        return std::unique_ptr<command_base_t>(new command_t<Callable>(fn) );
    }

    virtual void execute() {
        fn_();
    }
    Callable fn_;
};


class command_queue
{
public:
    explicit command_queue  (size_t queue_size = 128);

    void    execute_all     ();
    bool    execute_next    ();

//    template<class CommandCallable>
//    void push_cmd (CommandCallable&& command_fun)
//    {
//        push_cmd(command_t<CommandCallable>::create(std::forward(command_fun)));
////        push_cmd(std::unique_ptr<command_base_t>(new command_t(std::move(command_fun))));
//    }

    template<class CommandCallable>
    void push_cmd (const CommandCallable& command_fun)
    {
         push_cmd(command_t<CommandCallable>::create(command_fun));
    }

////////////////////

//    template<class CommandCallable,
//            typename ... CommandArgs>
//    void send (const CommandCallable& command_fun,
//              const CommandArgs&... args)
//    {
//        auto cmd = [=]() {
//            std::invoke(command_fun, args...);
//        };
//        push_cmd(std::move(cmd));
//    }

    template<class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void send (const CommandMemberCallable& command_member_fun,
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
    void send_callback(const std::function<void (const ReturnType&)>& callback_function,
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
    void send_callback (const CallbackMemberCallable& callback_member_fun,
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
    void send_callback(const CallbackMemberCallable& callback_member_fun,
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
    void send_response(command_queue& response_queue,
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
    void send_response(command_queue& response_queue,
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
    void send_response(command_queue& response_queue,
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
    void push_cmd           (std::unique_ptr<command_base_t> cmd);


    using queue_t       = estl::srsw_fifo<std::unique_ptr<command_base_t>>;

    queue_t         queue_;
    std::mutex      push_mutex_;

};


} // END namespace estl::asig

//////////////////////
//    template<class ReturnType>
//    void    push_callback(std::function<ReturnType ()>&&  command_fun,
//                          std::function<void (const ReturnType&)>&& result_callback_fun)
//    {
//        auto cmd = [=]() {
//            auto cmd_ret_val = command_fun();
//            result_callback_fun(cmd_ret_val);

//        };
//        push_cmd(std::move(cmd));
//    }

//    template<class ReturnType>
//    void    push_response(std::function<ReturnType ()>&&  command_fun,
//                          std::function<void (const ReturnType&)>&& result_callback_fun,
//                          std::shared_ptr<command_queue> response_queue
//                          )
//    {
//        auto cmd = [=]() {
//            auto cmd_ret_val = command_fun();
//            if (response_queue) {
//                auto cb = [=]() {
//                    result_callback_fun(cmd_ret_val);
//                };
//                response_queue->push_cmd(std::move(cb));
//            }
//            else {
//                result_callback_fun(cmd_ret_val);
//            }
//        };
//        push_cmd(std::move(cmd));
//    }

// ------------

//    template<typename ReturnType,
//             class CallbackCallable,
//             class CommandCallable,
//             typename ... CommandArgs >
//    void send_callback_xx(const CallbackCallable& callback_fun,
//                       const CommandCallable& command_fun,
//                       const CommandArgs&... command_args
//                       )
//    {
//        auto to_send_fn = [=](){
//            auto cmd_return_value = std::invoke(command_fun, command_args...);
//            std::invoke(callback_fun, cmd_return_value);
//        };
//        push_cmd(std::move(to_send_fn));
//    }

//    template<typename ReturnType,
//             class CallbackCallable,
//             class CommandCallable,
//             typename ... CommandArgs >
//    void send_callback_xx(const CallbackCallable& callback_fun,
//                       std::int32_t cmd_seq_num,
//                       const CommandCallable& command_fun,
//                       const CommandArgs&... command_args
//                       )
//    {
//        auto to_send_fn = [=](){
//            auto cmd_return_value = std::invoke(command_fun, command_args...);
//            std::invoke(callback_fun, cmd_return_value, cmd_seq_num);
//        };
//        push_cmd(std::move(to_send_fn));
//    }

//    template<class ReturnType,
//             class CallbackMemberCallable,
//             class CallbackClassObject,
//             class CommandCallable,
//             typename ... CommandArgs >
//    void send_callback (const CallbackMemberCallable& callback_member_fun,
//                       CallbackClassObject* callback_class_obj_ptr,
//                       const CommandCallable& command_fun,
//                       const CommandArgs&... command_args
//                       )
//    {
//        auto to_send_fn = [=](){
//            auto cmd_return_value = std::invoke(command_fun, command_args...);
//            std::invoke(callback_member_fun, callback_class_obj_ptr, cmd_return_value);
//        };
//        push_cmd(std::move(to_send_fn));
//    }

//
//    template<class ReturnType,
//             class CallbackCallable,
//             class CommandMemberCallable,
//             class CommandClassObject,
//             typename ... CommandArgs>
//    void send_callback (const CallbackCallable& callback_free_fun,
//                       const CommandMemberCallable& command_member_fun,
//                       CommandClassObject* command_class_obj_ptr,
//                       const CommandArgs&... command_args
//                       )
//    {
//        auto to_send_fn = [=](){
//            auto cmd_return_value = std::invoke(command_member_fun, command_class_obj_ptr, command_args...);
//            std::invoke(callback_free_fun, cmd_return_value);
//        };
//        push_cmd(std::move(to_send_fn));
//    }

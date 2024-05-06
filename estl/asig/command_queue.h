#pragma once

#include <iostream>
#include <mutex>
#include <functional>
#include <concurrent/srsw_fifo.hpp>

// https://en.cppreference.com/w/cpp/utility/functional/invoke
// https://codereview.stackexchange.com/questions/221828/c17-implementation-of-stdinvoke
// https://stackoverflow.com/questions/46388524/when-to-use-stdinvoke-instead-of-simply-calling-the-invokable

namespace estl {

class command_queue
{
public:
    explicit command_queue(size_t queue_size = 128);

    void    push_call_void  (std::function<void ()>&&  command_fun);

    template<class ReturnType>
    void    push_call       (std::function<ReturnType ()>&&  command_fun,
                             std::function<void (const ReturnType&)>&&  result_callback_fun)
    {
        auto fn = [=]() {
            std::cerr << "Executing with return type and callback\n";
            auto cmd_ret_val = command_fun();
            result_callback_fun(cmd_ret_val);

        };
        push_call_void(std::move(fn));
    }


    template<class CommandCallable, typename ... CommandArgs >
    void call_void (CommandCallable&& function, const CommandArgs&... args)
    {
        auto fn = [=]() {
            std::invoke(function, args...);
        };
        push_call_void(std::move(fn));
    }


    template<typename ReturnType,
             class ResultCallable,
             class CommandCallable, typename ... CommandArgs >
    void call ( ResultCallable result_callback_fun,
                CommandCallable command_fun, CommandArgs... command_args
                )
    {
        auto command_fn = [=]() -> ReturnType {
            return std::invoke(command_fun, command_args...);
        };

        auto result_callback_fn = [=](const ReturnType& cmd_return_value){
            return std::invoke(result_callback_fun, cmd_return_value );
        };
        push_call<ReturnType>(std::move(command_fn),  std::move(result_callback_fn));
    }

    template<typename ReturnType,
             class ResultMemberCallable, class ResultClassObject,
             class CommandCallable, typename ... CommandArgs >
    void call_memfun ( ResultMemberCallable result_callback_fun, ResultClassObject result_class_instance,
              CommandCallable command_fun, CommandArgs... command_args
              )
    {
        auto command_fn = [=]() -> ReturnType {
            return std::invoke(command_fun, command_args...);
        };

        auto result_callback_fn = [=](const ReturnType& cmd_return_value){
            return std::invoke(result_callback_fun, result_class_instance, cmd_return_value );
        };
        push_call<ReturnType>(std::move(command_fn),  std::move(result_callback_fn));
    }

    bool    execute_next    ();

    size_t  size            () const { return queue_.size();        }
    size_t  capacity        () const { return queue_.capacity();    }
    bool    empty           () const { return queue_.empty();       }


private:
    struct cmd_t{
        std::function<void ()>  command_fun;
    };

    using queue_t = estl::srsw_fifo<cmd_t>;

    queue_t     queue_;
    std::mutex  push_mutex_;

};


} // END namespace estl

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

    void    push            (std::function<void ()>&& command_fun);

    template<class ReturnType>
    void    push_callback(std::function<ReturnType ()>&&  command_fun,
                             std::function<void (const ReturnType&)>&& result_callback_fun)
    {
        auto cmd = [=]() {
            auto cmd_ret_val = command_fun();
            result_callback_fun(cmd_ret_val);

        };
        push(std::move(cmd));
    }

    template<class ReturnType>
    void    push_response(std::function<ReturnType ()>&&  command_fun,
                          std::function<void (const ReturnType&)>&& result_callback_fun,
                          std::shared_ptr<command_queue> response_queue
                          )
    {
        auto cmd = [=]() {
            auto cmd_ret_val = command_fun();
            if (response_queue) {
                auto cb = [=]() {
                    result_callback_fun(cmd_ret_val);
                };
                response_queue->push(std::move(cb));
            }
            else {
                result_callback_fun(cmd_ret_val);
            }
        };
        push(std::move(cmd));
    }

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


} // END namespace estl::asig

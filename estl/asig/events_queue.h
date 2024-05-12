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

namespace estl {

class events_queue
{
public:
    explicit events_queue(size_t queue_size = 128);

    template <class EventType>
    bool    push  (EventType event)
    {
        return queue_.push(event_data_t<EventType>::create(std::move(event)));
    }

//    template<class ReturnType>
//    void    push_callback(std::function<ReturnType ()>&&  command_fun,
//                             std::function<void (const ReturnType&)>&& result_callback_fun)
//    {
//        auto cmd = [=]() {
//            auto cmd_ret_val = command_fun();
//            result_callback_fun(cmd_ret_val);

//        };
//        push(std::move(cmd));
//    }

//    template<class ReturnType>
//    void    push_response(std::function<ReturnType ()>&&  command_fun,
//                            std::function<void (const ReturnType&)>&& result_callback_fun,
//                            std::shared_ptr<command_queue> response_queue
//                            )
//    {
//        auto cmd = [=]() {
//            auto cmd_ret_val = command_fun();
//            if (response_queue) {
//                auto cb = [=]() {
//                    result_callback_fun(cmd_ret_val);
//                };
//                response_queue->push(std::move(cb));
//            }
//            else {
//                result_callback_fun(cmd_ret_val);
//            }
//        };
//        push(std::move(cmd));
//    }


    bool    execute_next    ();

    size_t  size            () const { return queue_.size();        }
    size_t  capacity        () const { return queue_.capacity();    }
    bool    empty           () const { return queue_.empty();       }


private:
    struct event_data_base_t {
        virtual ~event_data_base_t() = default;
        virtual const void*       data        () const = 0;
    };

    template <class EventType>
    struct event_data_t : public event_data_base_t {
        event_data_t() = delete;
        explicit event_data_t(EventType event_data) : event_data_(std::move(event_data)) {}

        static std::unique_ptr<event_data_base_t> create(EventType event_data) {
            return std::unique_ptr<event_data_base_t>(new event_data_t<EventType>(std::move(event_data)) );
        }
        ~event_data_t() override = default;

        const void*         data   () const override        { return &event_data_;     }

        EventType   event_data_;
    };


    using queue_t = estl::srsw_fifo<std::unique_ptr<event_data_base_t>>;

    queue_t     queue_;
    std::mutex  push_mutex_;

};


} // END namespace estl



// template<class CommandCallable, typename ... CommandArgs>
// void send (CommandCallable&& function, const CommandArgs&... args)
// {
//     auto cmd = [=]() {
//         std::invoke(function, args...);
//     };
//     push(std::move(cmd));
// }

// template<typename ReturnType,
//          class CallbackCallable,
//          class CommandCallable,
//          class CommandClassObject,
//          typename ... CommandArgs >
// void send_callback(CallbackCallable callback_fun,
//                    CommandCallable command_member_fun,
//                    CommandClassObject* command_class_obj_ptr,
//                    CommandArgs... command_args
//                    )
// {
//     auto command_fn = [=]() -> ReturnType {
//         return std::invoke(command_member_fun, command_class_obj_ptr, command_args...);
//     };

//     auto callback_fn = [=](const ReturnType& cmd_return_value){
//         return std::invoke(callback_fun, cmd_return_value );
//     };
//     push_callback<ReturnType>(std::move(command_fn), std::move(callback_fn));
// }

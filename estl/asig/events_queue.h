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

class events_queue
{
public:
    struct event_data_base_t {
        virtual                 ~event_data_base_t  () = default;
        virtual const void*         data            () const = 0;
        virtual std::size_t         event_id        () const = 0;
        virtual const char*         name            () const = 0;
    };


    explicit events_queue(size_t queue_size = 128);

    template <class EventType>
    bool    push  (EventType event)
    {
        return queue_.push(event_data_t<EventType>::create(std::move(event)));
    }

    std::unique_ptr<event_data_base_t>
            pop_front       ();


    size_t  size            () const { return queue_.size();        }
    size_t  capacity        () const { return queue_.capacity();    }
    bool    empty           () const { return queue_.empty();       }


private:
    template <class EventType>
    struct event_data_t : public event_data_base_t {
        event_data_t() = delete;
        explicit event_data_t(EventType event_data) : event_data_(std::move(event_data)) {}

        static std::unique_ptr<event_data_base_t> create(EventType event_data) {
            return std::unique_ptr<event_data_base_t>(new event_data_t<EventType>(std::move(event_data)) );
        }
        ~event_data_t() override = default;

        const void*         data        () const override   { return &event_data_;                  }
        std::size_t         event_id    () const override   { return typeid(EventType).hash_code(); }
        const char*         name        () const override   { return typeid(EventType).name();      }

        EventType   event_data_;
    };


    using queue_t = estl::srsw_fifo<std::unique_ptr<event_data_base_t>>;

    queue_t     queue_;
    std::mutex  push_mutex_;

};


} // END namespace estl::asig

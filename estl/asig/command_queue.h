#pragma once

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

    void    push            (std::function<void ()>&&  command_fun);


    template<class Callable, typename ... Args>
    void    call            (Callable function, Args... args)
    {
        auto fn = [=]() {
            std::invoke(function, args...);
        };
        push(std::move(fn));
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

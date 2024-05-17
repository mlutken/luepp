#pragma once

#include <iostream>
#include <mutex>
#include <functional>
#include <map>
#include <memory>
#include <chrono>
#include <concurrent/srsw_fifo.hpp>


namespace estl::asig {

class timer_command_queue
{
public:
    using time_point = std::chrono::steady_clock::time_point;

    explicit timer_command_queue  (size_t queue_size = 64);

    void    execute_all     ();

    template<class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call_at(std::chrono::steady_clock::time_point call_at_time_point,
                 const CommandMemberCallable& command_member_fun,
                 CommandClassObject* command_class_obj_ptr,
                 const CommandArgs&... args)
    {
        auto cmd = [=]() {
            std::invoke(command_member_fun, command_class_obj_ptr, args...);
        };
        push_cmd(call_at_time_point, std::move(cmd));
    }

    template<class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call_in(std::chrono::milliseconds call_after_ms,
                 const CommandMemberCallable& command_member_fun,
                 CommandClassObject* command_class_obj_ptr,
                 const CommandArgs&... args)
    {
        const auto call_at_tp = std::chrono::steady_clock::now() + call_after_ms;
        call_at<CommandMemberCallable, CommandClassObject, CommandArgs...>
            (call_at_tp, command_member_fun, command_class_obj_ptr, args...);
    }


    size_t  size            () const { return input_queue_.size();        }
    size_t  capacity        () const { return input_queue_.capacity();    }
    bool    empty           () const { return input_queue_.empty();       }


private:
    struct timer_cmd_base_t {
        explicit timer_cmd_base_t(time_point call_at_tp) : call_at_tp_(call_at_tp) {}
        virtual ~timer_cmd_base_t() = default;
        virtual void execute() = 0;

        bool has_expired () const { return call_at_tp_ <= std::chrono::steady_clock::now(); }
        bool has_expired (time_point current_time_point) const { return call_at_tp_ <= current_time_point; }
        bool operator< (const timer_cmd_base_t& rhs) const { return call_at_tp_ < rhs.call_at_tp_; }

        time_point              call_at_tp_ {};
    };

    template<class Callable>
    struct timer_cmd_t : public timer_cmd_base_t {
        timer_cmd_t(time_point call_at_tp, Callable&& fn) :
            timer_cmd_base_t(call_at_tp), fn_(std::move(fn)) {}

        static std::unique_ptr<timer_cmd_base_t> create(time_point call_at_tp, Callable&& fn) {
            return std::unique_ptr<timer_cmd_base_t>(new timer_cmd_t<Callable>(call_at_tp, std::move(fn)) );
        }

        timer_cmd_t(time_point call_at_tp, const Callable& fn) :
            timer_cmd_base_t(call_at_tp), fn_(fn) {}

        static std::unique_ptr<timer_cmd_base_t> create(time_point call_at_tp, const Callable& fn) {
            return std::unique_ptr<timer_cmd_base_t>(new timer_cmd_t<Callable>(call_at_tp, fn) );
        }

        void execute() override {
            fn_();
        }
        Callable fn_;
    };

    template<class CommandCallable>
    void push_cmd (std::chrono::steady_clock::time_point call_at_tp, const CommandCallable& command_fun)
    {
        do_push_command(timer_cmd_t<CommandCallable>::create(call_at_tp, command_fun));
    }

    void do_push_command        (std::unique_ptr<timer_cmd_base_t> cmd);

    bool check_front_expired_and_execute    ();
    void process_input_queue    ();
    void put_in_pending_timers  (std::unique_ptr<timer_cmd_base_t> cmd, time_point current_time_point);
    void put_in_pending_timers  (std::unique_ptr<timer_cmd_base_t> cmd);


    using queue_t       = estl::srsw_fifo<std::unique_ptr<timer_cmd_base_t>>;
    using map_t         = std::map<time_point, std::unique_ptr<timer_cmd_base_t>>;

    queue_t         input_queue_;
    map_t           pending_timers_;
    std::mutex      input_queue_mutex_;

};

} // END namespace estl::asig


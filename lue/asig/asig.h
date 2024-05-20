#pragma once

#include <thread>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <functional>

#include <asig/commands.h>
#include <asig/events.h>


namespace lue::asig {


class asig
{
public:
    asig        ();
    asig        (size_t command_queues_size, size_t timer_queues_size, size_t event_queues_size);

    void        execute_all_for_this_thread ();

    void        register_receiver           (void* class_instance_ptr, std::thread::id thread_id = std::this_thread::get_id());

    size_t      command_queues_count        () const    { return commands_.command_queues_count();      }
    size_t      command_receivers_count     () const    { return commands_.command_receivers_count();   }
    size_t      command_queues_size         () const    { return commands_.command_queues_size();       }
    size_t      timer_queues_size           () const    { return commands_.timer_queues_size();         }

    // -----------------------------
    // --- Timer: Call functions ---
    // -----------------------------
    template<class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void timer_call_at(std::chrono::steady_clock::time_point call_at_time_point,
                       const CommandMemberCallable& command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       const CommandArgs&... args)
    {
        commands_.timer_call_at<CommandMemberCallable, CommandClassObject, CommandArgs...>
            (call_at_time_point, command_member_fun, command_class_obj_ptr, args...);
    }

    template<class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void timer_call_in(std::chrono::milliseconds call_after_ms,
                       const CommandMemberCallable& command_member_fun,
                       CommandClassObject* command_class_obj_ptr,
                       const CommandArgs&... args)
    {
        commands_.timer_call_in<CommandMemberCallable, CommandClassObject, CommandArgs...>
            (call_after_ms, command_member_fun, command_class_obj_ptr, args...);
    }

    // --------------------------------
    // --- Commands: Call functions ---
    // --------------------------------
    template<class CommandMemberCallable,
             class CommandClassObject,
             typename ... CommandArgs>
    void call(const CommandMemberCallable& command_member_fun,
              CommandClassObject* command_class_obj_ptr,
              const CommandArgs&... args)
    {
        commands_.call<CommandMemberCallable, CommandClassObject, CommandArgs...>
            (command_member_fun, command_class_obj_ptr, args...);
    }

    // ----------------------------------------------
    // --- Commands: Call with callback functions ---
    // ----------------------------------------------
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
        commands_.call_callback<ReturnType, CommandMemberCallable, CommandClassObject, CommandArgs...>
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
        commands_.call_callback<ReturnType, CallbackMemberCallable, CallbackClassObject, CommandMemberCallable, CommandClassObject, CommandArgs...>
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
        commands_.call_callback<ReturnType, CallbackMemberCallable, CallbackClassObject, CommandMemberCallable, CommandClassObject, CommandArgs...>
            (callback_member_fun, callback_class_obj_ptr, cmd_seq_num, command_member_fun, command_class_obj_ptr, command_args...);
    }

    // ----------------------------------------------
    // --- Commands: Call with response functions ---
    // ----------------------------------------------
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
        commands_.call_response<ReturnType, CommandMemberCallable, CommandClassObject, CommandArgs...>
            (response_function, command_member_fun, command_class_obj_ptr, command_args...);
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
        commands_.call_response<ReturnType, ResponseMemberCallable, ResponseClassObject, CommandMemberCallable, CommandClassObject, CommandArgs...>
            (response_member_fun, response_class_obj_ptr, command_member_fun, command_class_obj_ptr, command_args...);
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
        commands_.call_response<ReturnType, ResponseMemberCallable, ResponseClassObject, CommandMemberCallable, CommandClassObject, CommandArgs...>
            (response_member_fun, response_class_obj_ptr, cmd_seq_num, command_member_fun, command_class_obj_ptr, command_args...);
    }

    // ----------------------------
    // --- Events: Subscribe_to ---
    // ----------------------------
    template<class EventType>
    [[nodiscard]]
    event_subscription subscribe_to(std::function<void (const EventType&)>&& event_handler_fn,
                 std::thread::id thread_id = std::this_thread::get_id())
    {
        return events_.subscribe_to<EventType>(std::move(event_handler_fn), thread_id);
    }

    template<class EventType, class Callable>
    [[nodiscard]]
    event_subscription subscribe_to(Callable&& event_handler_fn,
                                    std::thread::id thread_id = std::this_thread::get_id())
    {
        return events_.subscribe_to<EventType, Callable>(std::move(event_handler_fn), thread_id);
    }


    template<class EventType,
             class MemberCallable,
             class ClassObject>
    [[nodiscard]]
    event_subscription subscribe_to(const MemberCallable& member_fun,
                                    ClassObject* class_obj_ptr,
                                    std::thread::id thread_id = std::this_thread::get_id())
    {
        return events_.subscribe_to<EventType, MemberCallable, ClassObject>
            (member_fun, class_obj_ptr, thread_id);
    }

    // -----------------------------------
    // --- Events: Subscribe_permanent ---
    // -----------------------------------
    template<class EventType>
    void subscribe_permanent(std::function<void (const EventType&)>&& event_handler_fn,
                             std::thread::id thread_id = std::this_thread::get_id())
    {
        events_.subscribe_permanent<EventType>(std::move(event_handler_fn), thread_id);
    }

    template<class EventType, class Callable>
    void subscribe_permanent(Callable&& event_handler_fn,
                             std::thread::id thread_id = std::this_thread::get_id())
    {
        events_.subscribe_permanent<EventType, Callable>(std::move(event_handler_fn), thread_id);
    }


    template<class EventType,
              class MemberCallable,
              class ClassObject>
    void subscribe_permanent(const MemberCallable& member_fun,
                             ClassObject* class_obj_ptr,
                             std::thread::id thread_id = std::this_thread::get_id())
    {
        events_.subscribe_permanent<EventType, MemberCallable, ClassObject>
            (member_fun, class_obj_ptr, thread_id);
    }


    // --------------------------
    // --- Events: unubscribe ---
    // --------------------------
    void un_subscribe (event_subscription &subscription, std::thread::id thread_id = std::this_thread::get_id());

    // -----------------------------
    // --- Events: publish_event ---
    // -----------------------------
    template<class EventType>
    void publish_event (const EventType& evt )
    {
        events_.publish_event<EventType>(evt);
    }

    size_t      event_subscribers_count     () const    { return events_.event_subscribers_count(); }


    void        dbg_print_command_receivers () const    { commands_.dbg_print_command_receivers(); }

private:
    commands    commands_;
    events      events_;
};


} // END namespace lue::asig

#pragma once

#include <iostream>
#include <thread>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <functional>
#include <type_traits>

#include <asig/events_queue.h>

// https://en.cppreference.com/w/cpp/utility/functional/invoke

namespace estl {
using event_id_t        = std::size_t;
using subscription_id_t = std::size_t;

class event_subscription {
    friend class events;
    event_subscription(std::size_t event_id, std::size_t subscription_id) : event_id_(event_id), subscription_id_(subscription_id) {}
public:
   event_subscription(const event_subscription& ) = default;
   event_subscription& operator=(const event_subscription& ) = default;

    event_subscription() = default;
    static constexpr std::size_t invalid_subscription_id = std::numeric_limits<std::size_t>::max();

    bool        is_valid        () const { return (event_id_ != 0) && (subscription_id_ != invalid_subscription_id); }

    std::size_t event_id        () const { return event_id_;        }
    std::size_t subscription_id () const { return subscription_id_; }
private:
    void        make_invalid    ();
    void        swap			(event_subscription& src) noexcept;



    std::size_t event_id_           = 0;
    std::size_t subscription_id_    = invalid_subscription_id;
};


class events
{
public:
    static constexpr std::size_t invalid_subscription_id = event_subscription::invalid_subscription_id;

public:

                events    ();
    explicit    events    (size_t event_queues_size);

    void        execute_all_for_this_thread ();


    template<class EventType>
    event_subscription subscribe_to_event(std::function<void (const EventType&)>&& event_handler_fn,
                                          std::thread::id thread_id = std::this_thread::get_id())
    {
        auto executor_ptr = std_function_executor_t<EventType>::create(std::move(event_handler_fn));
        const std::size_t event_id = typeid(EventType).hash_code();
        return do_subscribe_to_event(std::move(executor_ptr), event_id, thread_id);
    }

    template<class EventType, class Callable>
    event_subscription subscribe_to_event(Callable&& event_handler_fn,
                                          std::thread::id thread_id = std::this_thread::get_id())
    {
        auto executor_ptr = lambda_function_executor_t<EventType, Callable>::create(std::move(event_handler_fn));
        const std::size_t event_id = typeid(EventType).hash_code();
        return do_subscribe_to_event(std::move(executor_ptr), event_id, thread_id);
    }

    template<class EventType, class Callable>
    event_subscription subscribe_lambda(Callable&& event_handler_fn,
                                        std::thread::id thread_id = std::this_thread::get_id())
    {
        auto executor_ptr = lambda_function_executor_t<EventType, Callable>::create(std::move(event_handler_fn));
        const std::size_t event_id = typeid(EventType).hash_code();
        return do_subscribe_to_event(std::move(executor_ptr), event_id, thread_id);
    }

    template<class EventType,
             class CommandMemberCallable,
             class CommandClassObject>
    event_subscription subscribe_to_event(const CommandMemberCallable& command_member_fun,
                                          CommandClassObject* command_class_obj_ptr,
                                          std::thread::id thread_id = std::this_thread::get_id())
    {
        const std::size_t event_id = typeid(EventType).hash_code();
        std::cerr << "FIXMENM subscribe_to_event class version 1"
                  << " memfun: " << command_member_fun
                  << " class obj: " << command_class_obj_ptr
                  << " thread_id: " << thread_id
                  << " event_id: " << event_id
                  << "\n";

        auto event_handler_fn = [=](const EventType& event_data) {
            std::invoke(command_member_fun, command_class_obj_ptr, event_data);
        };

        return subscribe_to_event<EventType>(std::move(event_handler_fn), thread_id);

        // using mutable_mem_fun_ptr_t = void (CommandClassObject::*)(const EventType&);
        // using const_mem_fun_ptr_t = void (CommandClassObject::*)(const EventType&) const;

        // if constexpr (typeid(CommandMemberCallable) == typeid(const_mem_fun_ptr_t)) {
        //     std::cerr << "FIXMENM const\n";

        // }
        // else if constexpr(typeid(CommandMemberCallable) == typeid(mutable_mem_fun_ptr_t)) {
        //     std::cerr << "FIXMENM mutable\n";

        // }
        // static_assert(typeid(CommandMemberCallable) == typeid(mutable_mem_fun_ptr_t));

        // static_assert(typeid(CommandMemberCallable) == typeid(const_mem_fun_ptr_t));

        // const CommandMemberCallable* command_member_fun_ptr = &command_member_fun;
        // auto handler3 = std::function<void (const EventType&)>(
        //     [=](const EventType& e) {
        //         std::invoke(*command_member_fun_ptr, command_class_obj_ptr, e);
        //     });
        // return subscribe_to_event<EventType>(std::move(handler3));

        // std::invoke(command_member_fun, command_class_obj_ptr, EventType());

        // auto handler3 = std::function<void (const my_event_t&)>(
        //     [this](const my_event_t& e) {
        //         this->my_event_handler(e);
        //     });
        // my_event_subscription_3_ = event_center_.subscribe_to_event<my_event_t>(std::move(handler3));

        // auto event_handler_fn = [=](const EventType& event_data) {
        //     std::invoke(command_member_fun, command_class_obj_ptr, event_data);
        // };
        // auto executor_ptr = std_function_executor_t<EventType>::create(std::move(event_handler_fn));
        // return do_subscribe_to_event(std::move(executor_ptr), event_id, thread_id);


        // const CommandMemberCallable* command_member_fun_ptr = &command_member_fun;
        // static_assert(std::is_member_function_pointer_v<CommandMemberCallable>);
        // if constexpr (std::is_member_function_pointer_v<CommandMemberCallable>) {
        //     std::cerr << "FIXMENM is_member_function_pointer_v 1\n";

        // }

        // auto cmd = [=]() {


        // void (CommandClassObject::*command_member_fun_ptr)(const EventType&) const = command_member_fun;
        // (*command_class_obj_ptr.*command_member_fun_ptr)(EventType());

        // (*command_class_obj_ptr.*command_member_fun)(EventType());

        // auto executor_ptr = member_function_executor_t<EventType,CommandMemberCallable,CommandClassObject>::create(
        //     command_member_fun, command_class_obj_ptr);
        // return do_subscribe_to_event(std::move(executor_ptr), event_id, thread_id);

        // return event_subscription(); // FIXMENM
    }


    void un_subscribe (const event_subscription& subscription, std::thread::id thread_id = std::this_thread::get_id());

    template<class EventType>
    void publish_event (const EventType& evt )
    {
        std::cerr << "publish(): " << typeid(EventType).name() << "\n";
        auto subscriber_count_map = get_subscriber_count_per_thread(typeid(EventType).hash_code());
        for (const auto [thread_id, subscriber_count] : subscriber_count_map) {
            if (subscriber_count > 0) {
                auto data_queue_ptr = get_event_data_queue(thread_id);
                data_queue_ptr->push(evt);
            }
        }
    }

    size_t      subscribers_count           () const;

private:
    // -----------------------------
    // --- event_executor_base_t ---
    // -----------------------------
    struct event_executor_base_t {
        virtual ~event_executor_base_t      () = default;
        virtual std::size_t     event_id    () const = 0;
        virtual const char*     name        () const = 0;
        virtual void            execute     (const void* event_data_ptr) const = 0;
    };

    // -------------------------------
    // --- std_function_executor_t ---
    // -------------------------------
    template<class EventType, class Callable>
    struct lambda_function_executor_t : public event_executor_base_t {
        explicit lambda_function_executor_t(Callable&& fn) : fn_(std::move(fn)) {}

        static std::unique_ptr<event_executor_base_t> create(Callable&& fn) {
            return std::unique_ptr<event_executor_base_t>(new lambda_function_executor_t<EventType, Callable>(std::move(fn)) );
        }

        std::size_t     event_id    () const override        { return typeid(EventType).hash_code();    }
        const char*     name        () const override        { return typeid(EventType).name();         }

        void            execute     (const void* event_data_ptr) const override {
            if (!event_data_ptr) { return; }
            const EventType& event_data = *(static_cast<const EventType*>(event_data_ptr));
            fn_(event_data);
        }

        Callable fn_;
    };


    template <class EventType>
    struct std_function_executor_t : public event_executor_base_t {
        std_function_executor_t() = delete;
        explicit std_function_executor_t(std::function<void (const EventType&)>&& event_handler_fn)
            : event_handler_fn_(std::move(event_handler_fn)) {}
        static std::unique_ptr<event_executor_base_t> create(std::function<void (const EventType&)>&& event_handler_fn) {
            return std::unique_ptr<event_executor_base_t>(new std_function_executor_t<EventType>(std::move(event_handler_fn)) );
        }

        std::size_t     event_id    () const override        { return typeid(EventType).hash_code();    }
        const char*     name        () const override        { return typeid(EventType).name();         }

        void            execute     (const void* event_data_ptr) const override {
            if (!event_data_ptr) { return; }
            const EventType& event_data = *(static_cast<const EventType*>(event_data_ptr));
            event_handler_fn_(event_data);
        }
        std::function<void (const EventType&)>&& event_handler_fn_;
    };

    // template<class EventType,
    //         class CommandMemberCallable,
    //         class CommandClassObject>
    // struct member_function_executor_t : public event_executor_base_t {
    //     member_function_executor_t() = delete;

    //     explicit member_function_executor_t(const CommandMemberCallable& command_member_fun,
    //                                         CommandClassObject* command_class_obj_ptr)
    //         :   const_mem_fun_ptr_(command_member_fun),
    //             command_class_obj_ptr_(command_class_obj_ptr)
    //         {}

    //     static std::unique_ptr<event_executor_base_t> create(const CommandMemberCallable& command_member_fun,
    //                                                          CommandClassObject* command_class_obj_ptr)
    //     {
    //         std::cerr << "FIXMENM create: "
    //                   << "  command_class_obj_ptr_: " << command_class_obj_ptr
    //                   << "  const_mem_fun_ptr_: " << command_member_fun
    //                   << "\n";

    //         // const_mem_fun_ptr_t mem_fun_ptr = command_member_fun;
    //         // (*command_class_obj_ptr.*mem_fun_ptr)(EventType());
    //         // (*command_class_obj_ptr.*command_member_fun)(EventType());

    //         return std::unique_ptr<event_executor_base_t>(
    //             new member_function_executor_t<EventType, CommandMemberCallable, CommandClassObject>(command_member_fun, command_class_obj_ptr)
    //         );
    //     }
    //     ~member_function_executor_t() override = default;

    //     std::size_t     event_id    () const override        { return typeid(EventType).hash_code();    }
    //     const char*     name        () const override        { return typeid(EventType).name();         }

    //     void            execute     (const void* event_data_ptr) const override {
    //         if (!event_data_ptr) { return; }
    //         std::cerr << "FIXMENM execute event_data_ptr: " << event_data_ptr
    //                   << "  command_class_obj_ptr_: " << command_class_obj_ptr_
    //                   << "  const_mem_fun_ptr_: " << const_mem_fun_ptr_
    //                   << "\n";
    //         const EventType& event_data = *(static_cast<const EventType*>(event_data_ptr));
    //         // (*command_class_obj_ptr_.*const_mem_fun_ptr_)(EventType());
    //         (*command_class_obj_ptr_.*const_mem_fun_ptr_)(event_data);


    //         // const auto* command_member_fun_ptr = &command_member_fun_;
    //         // (*command_class_obj_ptr_.*command_member_fun_ptr)(event_data);

    //         // event_handler_fn_(event_data);
    //     }
    //     using const_mem_fun_ptr_t = void (CommandClassObject::*)(const EventType&) const;
    //     using mutable_mem_fun_ptr_t = void (CommandClassObject::*)(const EventType&);
    //     // void (CommandClassObject::*command_member_fun_ptr)(const EventType&) const = command_member_fun;
    //     // (*command_class_obj_ptr.*command_member_fun_ptr)(EventType());

    //     const_mem_fun_ptr_t const_mem_fun_ptr_{nullptr};
    //     mutable_mem_fun_ptr_t mutable_mem_fun_ptr_{nullptr};
    //     // const CommandMemberCallable& command_member_fun_;
    //     CommandClassObject* command_class_obj_ptr_{nullptr};
    // };

    struct executor_list_t {
        void            execute_all     (const void* event_data_ptr);
        void            unsubscribe     (std::size_t subscription_id);
        std::size_t     subscribe       (std::unique_ptr<event_executor_base_t> executor);
        std::size_t     size            () const    { return event_executors_.size(); }

    private:
        size_t          do_subscribe    (std::unique_ptr<event_executor_base_t> executor);
        void            do_unsubscribe  (std::size_t subscription_id);

        using subscription_id_vec_t         = std::vector<subscription_id_t>;
        using subscription_executor_vec_t   = std::vector<std::unique_ptr<event_executor_base_t>>;
        using event_executor_vec_t          = std::vector<std::unique_ptr<event_executor_base_t>>;
        mutable event_executor_vec_t        event_executors_        {};
        mutable subscription_executor_vec_t subscriptions_pending_  {};
        mutable subscription_id_vec_t       unsubscriptions_pending_{};
        mutable bool                        currently_executing_    {false};
    };


    using publish_data_queue_per_thread_map_t   = std::unordered_map<std::thread::id, std::shared_ptr<events_queue>>;

    using event_subscribers_map_t               = std::unordered_map<event_id_t, std::unique_ptr<executor_list_t>>;
    using evt_subscribers_lookup_t              = std::unordered_map<std::thread::id, std::shared_ptr<event_subscribers_map_t>>;
    using subscriber_count_per_thread_map_t     = std::unordered_map<std::thread::id, std::uint32_t>;
    using event_id_to_subscriber_count_t        = std::unordered_map<event_id_t, subscriber_count_per_thread_map_t>;

   // ---------------------------------
   // --- PRIVATE: Helper functions ---
   // ---------------------------------

    event_subscription do_subscribe_to_event(std::unique_ptr<event_executor_base_t> executor_ptr,
                                             std::size_t event_id,
                                             std::thread::id thread_id);

    std::shared_ptr<events_queue>
                        get_event_data_queue            (std::thread::id thread_id);

    std::shared_ptr<event_subscribers_map_t>
                        get_evt_subscribers_for_thread (std::thread::id thread_id);

    executor_list_t*    get_executor_list               (event_subscribers_map_t& thread_subscribers,  std::size_t event_id);

    subscriber_count_per_thread_map_t
                        get_subscriber_count_per_thread (std::size_t event_id) const;

    void                add_subscriber_for_thread       (std::size_t event_id, std::thread::id thread_id = std::this_thread::get_id());

    void                remove_subscriber_for_thread    (std::size_t event_id, std::thread::id thread_id = std::this_thread::get_id());


    mutable std::mutex                  publisher_thread_mutex_;
    mutable std::mutex                  subscriber_thread_mutex_;
    mutable std::mutex                  subscribers_per_thread_mutex_;

    size_t                              queues_size_                    = 128;


    publish_data_queue_per_thread_map_t publish_data_queue_per_thread_  {};
    evt_subscribers_lookup_t            event_subscribers_              {};
    event_id_to_subscriber_count_t      event_id_to_subscribed_threads_ {};


};


} // END namespace estl

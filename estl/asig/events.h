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

namespace estl::asig {
class events;
using event_id_t        = std::size_t;
using subscription_id_t = std::size_t;

class event_subscription {
    friend class events;
    event_subscription(std::size_t event_id, std::size_t subscription_id, events* events_ptr)
        : event_id_(event_id), subscription_id_(subscription_id), events_center_ptr_(events_ptr) {}
public:
    event_subscription(const event_subscription& rhs) = delete;
    event_subscription& operator=(const event_subscription& rhs) = delete;

    event_subscription(event_subscription&& moving);
    event_subscription& operator=(event_subscription&& moving);

    event_subscription() = default;
    ~event_subscription();

    static constexpr std::size_t invalid_subscription_id = std::numeric_limits<std::size_t>::max();

    bool        is_valid        () const { return (event_id_ != 0) && (subscription_id_ != invalid_subscription_id); }

    std::size_t event_id        () const { return event_id_;        }
    std::size_t subscription_id () const { return subscription_id_; }
private:
    void        make_invalid    ();
    void        swap			(event_subscription& src) noexcept;



    std::size_t event_id_           = 0;
    std::size_t subscription_id_    = invalid_subscription_id;
    events* events_center_ptr_      = {nullptr};
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
    [[nodiscard]]
    event_subscription subscribe_to(std::function<void (const EventType&)>&& event_handler_fn,
                                    std::thread::id thread_id = std::this_thread::get_id())
    {
        auto executor_ptr = std_function_executor_t<EventType>::create(std::move(event_handler_fn));
        const std::size_t event_id = typeid(EventType).hash_code();
        return do_subscribe_to(std::move(executor_ptr), event_id, thread_id);
    }

    template<class EventType, class Callable>
    [[nodiscard]]
    event_subscription subscribe_to(Callable&& event_handler_fn,
                                    std::thread::id thread_id = std::this_thread::get_id())
    {
        auto executor_ptr = lambda_function_executor_t<EventType, Callable>::create(std::move(event_handler_fn));
        const std::size_t event_id = typeid(EventType).hash_code();
        return do_subscribe_to(std::move(executor_ptr), event_id, thread_id);
    }


    template<class EventType,
             class MemberCallable,
             class ClassObject>
    [[nodiscard]]
    event_subscription subscribe_to(const MemberCallable& member_fun,
                                          ClassObject* class_obj_ptr,
                                          std::thread::id thread_id = std::this_thread::get_id())
    {
        auto event_handler_fn = [=](const EventType& event_data) {
            std::invoke(member_fun, class_obj_ptr, event_data);
        };

        return subscribe_to<EventType>(std::move(event_handler_fn), thread_id);
    }

    template<class EventType>
    void subscribe_permanent(std::function<void (const EventType&)>&& event_handler_fn,
                             std::thread::id thread_id = std::this_thread::get_id())
    {
        event_subscription subscription_ignore =
            subscribe_to<EventType>(std::move(event_handler_fn), thread_id);
        subscription_ignore.make_invalid();
    }

    template<class EventType, class Callable>
    void subscribe_permanent(Callable&& event_handler_fn,
                             std::thread::id thread_id = std::this_thread::get_id())
    {
        event_subscription subscription_ignore =
            subscribe_to<EventType, Callable>(std::move(event_handler_fn), thread_id);
        subscription_ignore.make_invalid();
    }


    template<class EventType,
              class MemberCallable,
              class ClassObject>
    void subscribe_permanent(const MemberCallable& member_fun,
                             ClassObject* class_obj_ptr,
                             std::thread::id thread_id = std::this_thread::get_id())
    {
        event_subscription subscription_ignore =
            subscribe_to<EventType, MemberCallable, ClassObject>(member_fun, class_obj_ptr, thread_id);
        subscription_ignore.make_invalid();
    }


    void un_subscribe (event_subscription &subscription, std::thread::id thread_id = std::this_thread::get_id());

    template<class EventType>
    void publish_event (const EventType& evt )
    {
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
                 event_executor_base_t      ();
        virtual ~event_executor_base_t      () = default;
        virtual std::size_t     event_id    () const = 0;
        virtual const char*     event_name  () const = 0;
        virtual const char*     handler_name() const = 0;
        virtual void            execute     (const void* event_data_ptr) const = 0;
        std::size_t             subscription_id_;
    private:
        static std::atomic_size_t   id_counter_;
    };

    // -------------------------------
    // --- std_function_executor_t ---
    // -------------------------------
    template<class EventType, class Callable>
    struct lambda_function_executor_t : public event_executor_base_t {
        explicit lambda_function_executor_t(Callable&& fn)
            : event_executor_base_t(), fn_(std::move(fn)) {}

        static std::unique_ptr<event_executor_base_t> create(Callable&& fn) {
            return std::unique_ptr<event_executor_base_t>(new lambda_function_executor_t<EventType, Callable>(std::move(fn)) );
        }

        std::size_t     event_id    () const override        { return typeid(EventType).hash_code();    }
        const char*     event_name  () const override        { return typeid(EventType).name();         }
        const char*     handler_name() const override        { return typeid(Callable).name();          }

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
            : event_executor_base_t(), event_handler_fn_(std::move(event_handler_fn)) {}
        static std::unique_ptr<event_executor_base_t> create(std::function<void (const EventType&)>&& event_handler_fn) {
            return std::unique_ptr<event_executor_base_t>(new std_function_executor_t<EventType>(std::move(event_handler_fn)) );
        }

        std::size_t     event_id    () const override        { return typeid(EventType).hash_code();    }
        const char*     event_name  () const override        { return typeid(EventType).name();         }
        const char*     handler_name() const override        { return typeid(std::function<void (const EventType&)>).name();         }

        void            execute     (const void* event_data_ptr) const override {
            if (!event_data_ptr) { return; }
            const EventType& event_data = *(static_cast<const EventType*>(event_data_ptr));
            event_handler_fn_(event_data);
        }
        std::function<void (const EventType&)>&& event_handler_fn_;
    };


    struct executor_list_t {
        void            execute_all         (const void* event_data_ptr);
        void            unsubscribe         (std::size_t subscription_id);
        std::size_t     subscribe           (std::unique_ptr<event_executor_base_t> executor);
        std::size_t     size                () const    { return event_executors_.size(); }
        std::size_t     next_subscription_id() const;


    private:
        size_t          do_subscribe    (std::unique_ptr<event_executor_base_t> executor);
        void            do_unsubscribe  (std::size_t subscription_id);

        using subscription_id_vec_t         = std::vector<subscription_id_t>;
        using event_executor_map_t          = std::unordered_map<std::size_t, std::unique_ptr<event_executor_base_t>>;
        using event_executor_vec_t          = std::vector<std::unique_ptr<event_executor_base_t>>;
        mutable event_executor_map_t        event_executors_        {};
        mutable event_executor_vec_t        subscriptions_pending_  {};
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

    event_subscription do_subscribe_to(std::unique_ptr<event_executor_base_t> executor_ptr,
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


} // END namespace estl::asig

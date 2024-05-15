#pragma once

#include <iostream>
#include <thread>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <functional>

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
        std::shared_ptr<event_subscribers_map_t> thread_subscribers = get_evt_subscribers_for_thread(thread_id);

        const std::size_t event_id = typeid(EventType).hash_code();

        executor_list_t* executor_list_ptr = get_executor_list(*thread_subscribers, event_id);
        std::size_t subscription_id = executor_list_ptr->subscribe(std::move(executor_ptr));
        add_subscriber_for_thread(event_id, thread_id);

        return event_subscription(event_id, subscription_id);
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
    struct event_executor_base_t {
        virtual ~event_executor_base_t      () = default;
        virtual std::size_t     event_id    () const = 0;
        virtual const char*     name        () const = 0;
        virtual void            execute     (const void* event_data_ptr) const = 0;
    };

    template <class EventType>
    struct std_function_executor_t : public event_executor_base_t {
        std_function_executor_t() = delete;
        explicit std_function_executor_t(std::function<void (const EventType&)>&& event_handler_fn)
            : event_handler_fn_(std::move(event_handler_fn)) {}
        static std::unique_ptr<event_executor_base_t> create(std::function<void (const EventType&)>&& event_handler_fn) {
            return std::unique_ptr<event_executor_base_t>(new std_function_executor_t<EventType>(std::move(event_handler_fn)) );
        }
        ~std_function_executor_t() override = default;

        std::size_t     event_id    () const override        { return typeid(EventType).hash_code();    }
        const char*     name        () const override        { return typeid(EventType).name();         }

        void            execute     (const void* event_data_ptr) const override {
            if (!event_data_ptr) { return; }
            const EventType& event_data = *(static_cast<const EventType*>(event_data_ptr));
            event_handler_fn_(event_data);
        }
        std::function<void (const EventType&)>&& event_handler_fn_;
    };

    struct executor_list_t {
        void            execute_all     (const void* event_data_ptr) const;
        void            unsubscribe     (std::size_t subscription_id);
        std::size_t     subscribe       (std::unique_ptr<event_executor_base_t> executor);
        std::size_t     size            () const    { return event_executors_.size(); }

    private:
        void            do_unsubscribe  (std::size_t subscription_id) const;

        using unsubscribe_vec_t     = std::vector<subscription_id_t>;
        using event_executor_vec_t  = std::vector<std::unique_ptr<event_executor_base_t>>;
        mutable event_executor_vec_t    event_executors_        {};
        mutable unsubscribe_vec_t       unsubscriptions_pending_{};
        mutable bool                    currently_executing_    {false};

    };


    using publish_data_queue_per_thread_map_t   = std::unordered_map<std::thread::id, std::shared_ptr<events_queue>>;

    using event_subscribers_map_t               = std::unordered_map<event_id_t, std::unique_ptr<executor_list_t>>;
    using evt_subscribers_lookup_t              = std::unordered_map<std::thread::id, std::shared_ptr<event_subscribers_map_t>>;
    using subscriber_count_per_thread_map_t     = std::unordered_map<std::thread::id, std::uint32_t>;
    using event_id_to_subscriber_count_t        = std::unordered_map<event_id_t, subscriber_count_per_thread_map_t>;


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

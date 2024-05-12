#pragma once

#include <thread>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <functional>


// https://en.cppreference.com/w/cpp/utility/functional/invoke

namespace estl {
class event_subscription {
    friend class events;
    event_subscription(std::size_t event_id, std::size_t subscription_id) : event_id_(event_id), subscription_id_(subscription_id) {}
public:
    event_subscription() = default;
    static constexpr std::size_t invalid_subscription_id = std::numeric_limits<std::size_t>::max();

    bool        is_valid        () const { return (event_id_ != 0) && (subscription_id_ != invalid_subscription_id); }

    std::size_t event_id        () const { return event_id_;        }
    std::size_t subscription_id () const { return subscription_id_; }
private:
    std::size_t event_id_           = 0;
    std::size_t subscription_id_    = invalid_subscription_id;
};


class events
{
public:
    static constexpr std::size_t invalid_subscription_id = std::numeric_limits<std::size_t>::max();
private:
    struct event_executor_base_t {
        virtual ~event_executor_base_t() = default;
        virtual std::size_t     event_id   () const = 0;
        virtual void            execute    (const void* event_data_ptr) const = 0;
    };

    template <class EventType>
    struct event_executor_t : public event_executor_base_t {
    private:
        event_executor_t() = delete;
        explicit event_executor_t(std::function<void (const EventType&)>&& event_handler_fn)
            : event_handler_fn_(std::move(event_handler_fn)) {}
    public:
        static std::unique_ptr<event_executor_base_t> create(std::function<void (const EventType&)>&& event_handler_fn) {
            return std::unique_ptr<event_executor_base_t>(new event_executor_t<EventType>(std::move(event_handler_fn)) );
        }
        ~event_executor_t() override = default;

        std::size_t     event_id   () const override        { return typeid(EventType).hash_code();     }
        void            execute    (const void* event_data_ptr) const override {
            if (!event_data_ptr) { return; }
            const EventType& event_data = *(static_cast<const EventType*>(event_data_ptr));
            event_handler_fn_(event_data);
        }
        std::function<void (const EventType&)>&& event_handler_fn_;
    };

    struct executor_list_t {
        void            execute_all     (const void* event_data_ptr) const {
            currently_executing_ = true;
            for (const auto& evt_exe_ptr: event_executors_) {
                evt_exe_ptr->execute(event_data_ptr);
            }
            currently_executing_ = false;
        }
        void            unsubscribe     (std::size_t subscription_id) {
            if (currently_executing_) { return; }
            if (subscription_id < event_executors_.size() ) {
                const auto erase_it = event_executors_.begin() + static_cast<std::int64_t>(subscription_id);
                event_executors_.erase(erase_it);
            }
        }
        std::size_t     subscribe       (std::unique_ptr<event_executor_base_t> executor) {
            if (currently_executing_)   { return invalid_subscription_id; }
            event_executors_.push_back(std::move(executor));
            const auto subscription_id = std::distance(event_executors_.begin(), (event_executors_.end() -1));
            return static_cast<std::size_t>(subscription_id);
        }

        using event_executor_vec_t = std::vector<std::unique_ptr<event_executor_base_t>>;
        event_executor_vec_t    event_executors_        {};
        mutable bool            currently_executing_    {false};
    };

    using per_thread_evt_subscribers_map_t = std::unordered_map<std::size_t, std::unique_ptr<executor_list_t>>;

    using evt_subscribers_lookup_t  = std::unordered_map<std::thread::id, std::shared_ptr<per_thread_evt_subscribers_map_t>>;
    evt_subscribers_lookup_t    event_subscribers_;

    std::shared_ptr<per_thread_evt_subscribers_map_t>
                                get_evt_subscribers_for_thread (std::thread::id thread_id) {
        std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
        auto it = event_subscribers_.find(thread_id);
        if (it != event_subscribers_.end()) {
            return it->second;
        }
        auto ptr = std::make_shared<per_thread_evt_subscribers_map_t>();
        event_subscribers_[thread_id] = ptr;
        return ptr;
    }

    executor_list_t*    get_executor_list (per_thread_evt_subscribers_map_t & thread_subscribers,  std::size_t event_id) {
        auto it = thread_subscribers.find(event_id);
        if (it == thread_subscribers.end()) {
            auto pair = thread_subscribers.insert_or_assign(event_id, std::make_unique<executor_list_t>());
            it = pair.first;
        }
        return it->second.get();
    }

public:

    events    ();
    explicit    events    (size_t event_queues_size);


    template<class EventType>
    event_subscription subscribe_to_event (std::function<void (const EventType&)>&& event_handler_fn, std::thread::id thread_id = std::this_thread::get_id())
    {
        std::shared_ptr<per_thread_evt_subscribers_map_t> thread_subscribers = get_evt_subscribers_for_thread(thread_id);

        const std::size_t event_id = typeid(EventType).hash_code();


        executor_list_t* executor_list_ptr = get_executor_list(*thread_subscribers, event_id);
        auto executor_ptr = event_executor_t<EventType>::create(std::move(event_handler_fn));
        std::size_t subscription_id = executor_list_ptr->subscribe(std::move(executor_ptr));

        return event_subscription(event_id, subscription_id);
    }


    void un_subscribe (const event_subscription& subscription, std::thread::id thread_id = std::this_thread::get_id()) {
        if (!subscription.is_valid()) { return; }
        std::shared_ptr<per_thread_evt_subscribers_map_t> thread_subscribers = get_evt_subscribers_for_thread(thread_id);
        executor_list_t* executor_list_ptr = get_executor_list(*thread_subscribers, subscription.event_id());
        executor_list_ptr->unsubscribe(subscription.subscription_id());
    }


    template<class EventType>
    void publish_event (const EventType& evt )
    {
        // std::cerr << "publish(): " << evt << "\n";
        // auto cmd_queue = get_receiver_queue(command_class_obj);
        // if (!cmd_queue) {
        //     return;
        // }
        // auto cmd = [=]() {
        //     return std::invoke(command_fun, command_class_obj, command_args...);
        // };

        // command_queue& cc = *cmd_queue;
        // cc.push(std::move(cmd));
    }

    //-----------------




//    void        register_command_receiver   (void* class_instance_ptr, std::thread::id thread_id = std::this_thread::get_id());
//    queue_ptr_t get_receiver_queue          (std::thread::id thread_id = std::this_thread::get_id());
//    queue_ptr_t get_receiver_queue          (void* class_instance_ptr);

//    size_t      queues_count                () const;
//    size_t      receivers_count             () const;
//    size_t      queues_size                 () const { return command_queues_size_; }

//    void        dbg_print_command_receivers () const;

private:
///    using receiver_lookup_map_t     = std::unordered_map<void*, std::thread::id>;

    mutable std::mutex      thread_lookup_mutex_;
    size_t                  command_queues_size_ = 128;


};


} // END namespace estl


//struct executor_list_base_t {
//    virtual ~executor_list_base_t              () = default;
//    virtual std::size_t     event_id        () const = 0;
//    virtual void            execute_all     (const void* event_data_ptr) const = 0;
//    virtual void            unsubscribe     (std::size_t subscription_id) = 0;
//};

//template <class EventType>
//struct executor_list_t : public executor_list_base_t {
//    ~executor_list_t() override = default;
//    std::size_t     event_id   () const override        { return typeid(EventType).hash_code();     }
//    void            execute_all     (const void* event_data_ptr) const override {
//        currently_executing_ = true;
//        for (const auto& evt_exe_ptr: event_executors_) {
//            evt_exe_ptr->execute(event_data_ptr);
//        }
//        currently_executing_ = false;
//    }
//    void            unsubscribe     (std::size_t subscription_id) {
//        if (currently_executing_) { return; }
//        if (subscription_id < event_executors_.size() ) {
//            event_executors_.erase(event_executors_.begin()+subscription_id);
//        }
//    }
//    std::size_t     subscribe       (std::function<void (const EventType&)>&& event_handler_fn) {
//        if (currently_executing_)   { return invalid_subscription_id; }
//        auto executor = std::make_unique<event_executor_t<EventType>>(std::move(event_handler_fn));
//        event_executors_.push_back(std::move(executor));
//        return std::distance(event_executors_.begin(), (event_executors_.end() -1));
//    }

//    using event_executor_vec_t = std::vector<std::unique_ptr<event_executor_t<EventType>>>;
//    event_executor_vec_t    event_executors_        {};
//    mutable bool            currently_executing_    {false};
//};

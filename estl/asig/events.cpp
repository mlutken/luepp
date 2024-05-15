#include "events.h"

#include <iostream>
#include <cassert>

using namespace std;

namespace estl {

// --------------------------
// --- event_subscription ---
// --------------------------

// event_subscription::event_subscription(const event_subscription& rhs) :
//     event_id_           (rhs.event_id_),
//     subscription_id_    (rhs.subscription_id_),
//     events_center_ptr_  (rhs.events_center_ptr_)
// {

// }

// event_subscription& event_subscription::operator=(const event_subscription& rhs)
// {
//     event_id_           = rhs.event_id_;
//     subscription_id_    = rhs.subscription_id_;
//     events_center_ptr_  = rhs.events_center_ptr_;

//     return *this;
// }

event_subscription::event_subscription(event_subscription&& moving)
{
    moving.swap(*this);
}

event_subscription& event_subscription::operator=(event_subscription&& moving)
{
    moving.swap(*this);
    return *this;
}

event_subscription::~event_subscription()
{
    if (events_center_ptr_) {
        std::cerr << "FIXMENM event_subscription::DESTRUCTOR, events_center_ptr_: " << events_center_ptr_ << "\n";
        events_center_ptr_->un_subscribe(*this);
    }
}

void event_subscription::make_invalid()
{
    subscription_id_ = invalid_subscription_id;
    events_center_ptr_ = nullptr;
}

void event_subscription::swap(event_subscription& src) noexcept
{
    std::swap(event_id_, src.event_id_);
    std::swap(subscription_id_, src.subscription_id_);
    std::swap(events_center_ptr_, src.events_center_ptr_);

}

// ---------------------------------
// --- events: main class PUBLIC ---
// ---------------------------------


events::events():
    publisher_thread_mutex_(),
    subscriber_thread_mutex_(),
    subscribers_per_thread_mutex_()
{

}

events::events(size_t command_queues_size) :
    publisher_thread_mutex_(),
    subscriber_thread_mutex_(),
    subscribers_per_thread_mutex_(),
    queues_size_(command_queues_size)
{

}

void events::execute_all_for_this_thread()
{
    const std::thread::id thread_id = std::this_thread::get_id();
    auto events_queue = get_event_data_queue(thread_id);
    std::shared_ptr<event_subscribers_map_t> thread_subscribers = get_evt_subscribers_for_thread(thread_id);

    while ( auto event_data_ptr = events_queue->pop_front()) {
        const auto event_id = event_data_ptr->event_id();
        executor_list_t* executor_list_ptr = get_executor_list(*thread_subscribers, event_id);
        if (executor_list_ptr) {
            executor_list_ptr->execute_all(event_data_ptr->data());
        }
    }

}

void events::un_subscribe(event_subscription& subscription, std::thread::id thread_id) {
    if (!subscription.is_valid()) {
        return;
    }
    std::shared_ptr<event_subscribers_map_t> thread_subscribers = get_evt_subscribers_for_thread(thread_id);
    executor_list_t* executor_list_ptr = get_executor_list(*thread_subscribers, subscription.event_id());
    executor_list_ptr->unsubscribe(subscription.subscription_id());
    remove_subscriber_for_thread(subscription.event_id(), thread_id);
    subscription.make_invalid();
}

size_t events::subscribers_count() const
{
    size_t count = 0;
    std::scoped_lock<std::mutex> lock(subscriber_thread_mutex_);
    for (auto [thread_id, evt_subscr_map_ptr] : event_subscribers_) {
        for (const auto& pair : *evt_subscr_map_ptr) {
            const std::unique_ptr<executor_list_t>& exe_list_ptr = pair.second;
            count += exe_list_ptr->size();
        }
    }

    return count;
}

event_subscription events::do_subscribe_to(std::unique_ptr<event_executor_base_t> executor_ptr,
                                                 std::size_t event_id,
                                                 std::thread::id thread_id)
{
    std::shared_ptr<event_subscribers_map_t> thread_subscribers = get_evt_subscribers_for_thread(thread_id);
    executor_list_t* executor_list_ptr = get_executor_list(*thread_subscribers, event_id);
    std::size_t subscription_id = executor_list_ptr->subscribe(std::move(executor_ptr));
    add_subscriber_for_thread(event_id, thread_id);

    return event_subscription(event_id, subscription_id, this);
}

// ----------------------------------------------------
// --- events::executor_list_t nested class PRIVATE ---
// ----------------------------------------------------

std::shared_ptr<events_queue> events::get_event_data_queue(std::thread::id thread_id)
{
    std::scoped_lock<std::mutex> lock(publisher_thread_mutex_);
    const auto it_queue = publish_data_queue_per_thread_.find(thread_id);
    if (it_queue != publish_data_queue_per_thread_.end()) {
        return it_queue->second;
    }
    auto queue_ptr = std::make_shared<events_queue>();
    publish_data_queue_per_thread_[thread_id] = queue_ptr;
    return queue_ptr;
}

void events::executor_list_t::execute_all(const void* event_data_ptr) {
    subscriptions_pending_.clear();
    unsubscriptions_pending_.clear();
    currently_executing_ = true;
    // std::size_t id = 0;
    for (const auto& evt_exe_ptr: event_executors_) {
        // assert (id++ == evt_exe_ptr->subscription_id_);
        evt_exe_ptr->execute(event_data_ptr);
    }
    for (auto subscription_id : unsubscriptions_pending_) {
        do_unsubscribe(subscription_id);
    }
    subscription_id_vec_t new_subscriptions;
    for (size_t i = 0; i < subscriptions_pending_.size(); ++i) {
        auto& executor = subscriptions_pending_[i];
        // We have set an expected subscription id, which really should macth
        if ( (next_subscription_id() +i) == executor->subscription_id_) {
            std::cerr << " **** FIXMENM OK subscription ID: " << executor->subscription_id_ << "\n";
        }
        const auto subscription_id = do_subscribe(std::move(executor));
        new_subscriptions.push_back(subscription_id);
    }
    currently_executing_ = false;
}

void events::executor_list_t::unsubscribe(std::size_t subscription_id) {
    if (currently_executing_) {
        unsubscriptions_pending_.push_back(subscription_id);
        return;
    }
    do_unsubscribe(subscription_id);
}

size_t events::executor_list_t::subscribe(std::unique_ptr<event_executor_base_t> executor)  {
    if (currently_executing_)   {
        subscriptions_pending_.push_back(std::move(executor));
        return invalid_subscription_id;
    }
    return do_subscribe(std::move(executor));
}

size_t events::executor_list_t::next_subscription_id() const
{
    return static_cast<std::size_t>( std::distance(event_executors_.begin(), event_executors_.end()) );
}

size_t events::executor_list_t::do_subscribe (std::unique_ptr<event_executor_base_t> executor)
{
    const auto subscription_id = next_subscription_id();
    executor->subscription_id_ = subscription_id;
    event_executors_.push_back(std::move(executor));
    // const auto subscription_id = static_cast<std::size_t>(std::distance(event_executors_.begin(), (event_executors_.end() -1)));
    return subscription_id;
}

void events::executor_list_t::do_unsubscribe(std::size_t subscription_id)
{
    if (subscription_id < event_executors_.size() ) {
        const auto erase_it = event_executors_.begin() + static_cast<std::int64_t>(subscription_id);
        event_executors_.erase(erase_it);
    }
}

// ----------------------------------
// --- events: main class PRIVATE ---
// ----------------------------------

std::shared_ptr<events::event_subscribers_map_t>
events::get_evt_subscribers_for_thread(std::thread::id thread_id) {
    std::scoped_lock<std::mutex> lock(subscriber_thread_mutex_);
    auto it = event_subscribers_.find(thread_id);
    if (it != event_subscribers_.end()) {
        return it->second;
    }
    auto ptr = std::make_shared<event_subscribers_map_t>();
    event_subscribers_[thread_id] = ptr;
    return ptr;
}

events::executor_list_t* events::get_executor_list(event_subscribers_map_t& thread_subscribers, std::size_t event_id) {
    auto it = thread_subscribers.find(event_id);
    if (it == thread_subscribers.end()) {
        auto pair = thread_subscribers.insert_or_assign(event_id, std::make_unique<executor_list_t>());
        it = pair.first;
    }
    return it->second.get();
}

events::subscriber_count_per_thread_map_t events::get_subscriber_count_per_thread(std::size_t event_id) const
{
    std::scoped_lock<std::mutex> lock(subscribers_per_thread_mutex_);
    const auto it = event_id_to_subscribed_threads_.find(event_id);
    if (it != event_id_to_subscribed_threads_.end()) {
        return it->second;
    }
    return subscriber_count_per_thread_map_t();
}

void events::add_subscriber_for_thread(std::size_t event_id, std::thread::id thread_id)
{
    std::scoped_lock<std::mutex> lock(subscribers_per_thread_mutex_);
    auto& subscriber_counts_per_thread = event_id_to_subscribed_threads_[event_id];
 ///    ++subscriber_counts_per_thread[thread_id]; // TODO: Is this enough, instead of the lines below ?
    auto it = subscriber_counts_per_thread.find(thread_id);
    if (it != subscriber_counts_per_thread.end()) {
        auto& subscriber_count = it->second;
        ++subscriber_count;
    }
    else {
        subscriber_counts_per_thread[thread_id] = 1;
    }
}

void events::remove_subscriber_for_thread(std::size_t event_id, std::thread::id thread_id)
{
    std::scoped_lock<std::mutex> lock(subscribers_per_thread_mutex_);
    auto& subscriber_counts_per_thread = event_id_to_subscribed_threads_[event_id];
    auto it = subscriber_counts_per_thread.find(thread_id);
    if (it != subscriber_counts_per_thread.end()) {
        auto& subscriber_count = it->second;
        if (subscriber_count > 0) {
            --subscriber_count;
        }
    }
}

} // END namespace estl

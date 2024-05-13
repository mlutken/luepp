#include "events.h"

#include <iostream>

using namespace std;

namespace estl {

// ------------------------------
// --- events: nested classes ---
// ------------------------------


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

        std::cerr << "FIXMENM Executing event name: " << event_data_ptr->name()
                  << ", id: " << event_id
                  << ", exe list size: " << executor_list_ptr->size()
                  << "\n";
        if (executor_list_ptr) {
            executor_list_ptr->execute_all(event_data_ptr->data());
        }
    }

}

void events::un_subscribe(const event_subscription& subscription, std::thread::id thread_id) {
    if (!subscription.is_valid()) { return; }
    std::shared_ptr<event_subscribers_map_t> thread_subscribers = get_evt_subscribers_for_thread(thread_id);
    executor_list_t* executor_list_ptr = get_executor_list(*thread_subscribers, subscription.event_id());
    executor_list_ptr->unsubscribe(subscription.subscription_id());
    remove_subscriber_for_thread(subscription.event_id(), thread_id);

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

void events::executor_list_t::execute_all(const void* event_data_ptr) const {
    currently_executing_ = true;
    for (const auto& evt_exe_ptr: event_executors_) {
        evt_exe_ptr->execute(event_data_ptr);
    }
    currently_executing_ = false;
}

void events::executor_list_t::unsubscribe(std::size_t subscription_id) {
    if (currently_executing_) { return; }
    if (subscription_id < event_executors_.size() ) {
        const auto erase_it = event_executors_.begin() + static_cast<std::int64_t>(subscription_id);
        event_executors_.erase(erase_it);
    }
}

size_t events::executor_list_t::subscribe(std::unique_ptr<event_executor_base_t> executor) {
    if (currently_executing_)   { return invalid_subscription_id; }
    event_executors_.push_back(std::move(executor));
    const auto subscription_id = std::distance(event_executors_.begin(), (event_executors_.end() -1));
    return static_cast<std::size_t>(subscription_id);
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



//size_t events::queues_count() const
//{
//    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
//    return cmd_queues_.size();
//}

//size_t events::receivers_count() const
//{
//    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
//    return receiver_lookup_.size();
//}

//void events::dbg_print_command_receivers() const
//{
//    std::scoped_lock<std::mutex> lock(thread_lookup_mutex_);
//    cerr << "*** Printing all receiver classes, sorted per thread ***\n";
//    for (auto& [print_thread_id, queue_ptr]: cmd_queues_) {
//        cerr << "[thread id: " << print_thread_id << "]\n";

//        for (auto& [class_ptr, look_fo_thread_id] : receiver_lookup_) {
//            if (print_thread_id == look_fo_thread_id) {
//                cerr << "   Class: " << class_ptr << "\n";
//            }
//        }
//    }
//}



} // END namespace estl

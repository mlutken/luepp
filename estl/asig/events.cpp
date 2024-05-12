#include "events.h"

#include <iostream>

using namespace std;

namespace estl {

events::events()
    :   thread_lookup_mutex_()
{

}

events::events(size_t command_queues_size)
    :   thread_lookup_mutex_(),
        command_queues_size_(command_queues_size)
{

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

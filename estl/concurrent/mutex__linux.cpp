#include "mutex.hpp"

namespace estl {

mutex::mutex() NESTLE_NOEXEPT
{
    pthread_mutex_init(&pthread_mutex_, nullptr);
}

mutex::~mutex()
{
    pthread_mutex_destroy(&pthread_mutex_);
}

void mutex::lock()
{
    pthread_mutex_lock(&pthread_mutex_);
}

bool mutex::try_lock()
{
    return pthread_mutex_trylock(&pthread_mutex_) == 0;
}

void mutex::unlock()
{
    pthread_mutex_unlock(&pthread_mutex_);
}


} // END namespace estl




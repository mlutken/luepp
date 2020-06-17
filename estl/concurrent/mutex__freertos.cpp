#include "mutex.hpp"

namespace estl {

mutex::mutex() NESTLE_NOEXEPT
{
    ensure_created(); // TODO: Should we make this lazy?? Perhaps due problems with FreeRTOS call during static initialization!!
}

mutex::~mutex()
{
    if (!semaphore_) {
        vSemaphoreDelete(semaphore_);
    }
}

void mutex::lock()
{
    //ensure_created(); // TODO: Should we do this ?? Perhaps due problems with FreeRTOS call during static initialization!!
    xSemaphoreTake(semaphore_, portMAX_DELAY);
}

bool mutex::try_lock()
{
    //ensure_created(); // TODO: Should we do this ?? Perhaps due problems with FreeRTOS call during static initialization!!
    return xSemaphoreTake(semaphore_, 0) == pdTRUE;
}

void mutex::unlock()
{
    xSemaphoreGive(semaphore_);
}

void mutex::ensure_created()
{
    if (!semaphore_) {
        semaphore_ = xSemaphoreCreateMutex();
    }
}


} // END namespace estl




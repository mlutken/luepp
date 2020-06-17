#include <nestle_default_config.h>


#if (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_LINUX)
#   include "mutex__linux.cpp"
#elif (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_WINDOWS)
#   include "mutex__windows.cpp"
#elif (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_OSX)
#   include "mutex__osx.cpp"
#elif (NESTLE_SYSTEM_NAME == NESTLE_SYSTEM_NAME_FREERTOS)
#   include "mutex__freertos.cpp"
#else
#   error "Missing estl::mutex platform implementation file!"
#endif // (NESTLE_SYSTEM_NAME)

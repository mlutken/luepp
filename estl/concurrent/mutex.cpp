#include <luepp_default_config.h>


#if (LUEPP_SYSTEM_NAME == LUEPP_SYSTEM_NAME_LINUX)
#   include "mutex__linux.cpp"
#elif (LUEPP_SYSTEM_NAME == LUEPP_SYSTEM_NAME_WINDOWS)
#   include "mutex__windows.cpp"
#elif (LUEPP_SYSTEM_NAME == LUEPP_SYSTEM_NAME_OSX)
#   include "mutex__osx.cpp"
#elif (LUEPP_SYSTEM_NAME == LUEPP_SYSTEM_NAME_FREERTOS)
#   include "mutex__freertos.cpp"
#else
#   error "Missing estl::mutex platform implementation file!"
#endif // (LUEPP_SYSTEM_NAME)

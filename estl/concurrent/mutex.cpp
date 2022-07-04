#include <luepp_platform_definitions.h>
#include <luepp_default_config.h>


#if (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_LINUX)
#   include "mutex__linux.cpp"
#elif (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_WINDOWS)
#   include "mutex__windows.cpp"
#elif (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_OSX)
#   include "mutex__osx.cpp"
#elif (LUEPP_SYSTEM_ID == LUEPP_SYSTEM_ID_FREERTOS)
#   include "mutex__freertos.cpp"
#else
#   error "Missing estl::mutex platform implementation file!"
#endif // (LUEPP_SYSTEM_ID)

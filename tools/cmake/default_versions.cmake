set (BOOST_MIN_VERSION 1.55)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# -----------------------------
# --- Defines documentation ---
# -----------------------------
# IMPORTANT: Must match the definitions set in config/nestle_default_config.h
#            The reason for the redundancy is to make it easier for users that do not want
#            to compile this using cmake!
set (NESTLE_PLATFORM_TYPE_DESKTOP   1)
set (NESTLE_PLATFORM_TYPE_EMBEDDED  2)

set (NESTLE_SYSTEM_NAME_LINUX       1)
set (NESTLE_SYSTEM_NAME_WINDOWS     2)
set (NESTLE_SYSTEM_NAME_OSX         3)
set (NESTLE_SYSTEM_NAME_FREERTOS    4)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set (NESTLE_PLATFORM_TYPE ${NESTLE_PLATFORM_TYPE_DESKTOP})
set (NESTLE_SYSTEM_NAME   ${NESTLE_SYSTEM_NAME_LINUX})
# TODO: Other platforms like windows and osx
endif ()

# message ("NESTLE_PLATFORM_TYPEID: '${NESTLE_PLATFORM_TYPEID}'")
# message ("NESTLE_PLATFORM_NAMEID: '${NESTLE_PLATFORM_NAMEID}'")


# -----------------------------
# --- Defines documentation ---
# -----------------------------
# IMPORTANT: Must match the definitions set in config/luepp_default_config.h
#            The reason for the redundancy is to make it easier for users that do not want
#            to compile this using cmake!
set (LUEPP_PLATFORM_TYPE_DESKTOP   1)
set (LUEPP_PLATFORM_TYPE_EMBEDDED  2)

set (LUEPP_SYSTEM_NAME_LINUX       1)
set (LUEPP_SYSTEM_NAME_WINDOWS     2)
set (LUEPP_SYSTEM_NAME_OSX         3)
set (LUEPP_SYSTEM_NAME_FREERTOS    4)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set_default (LUEPP_PLATFORM_TYPE ${LUEPP_PLATFORM_TYPE_DESKTOP})
set_default (LUEPP_SYSTEM_NAME   ${LUEPP_SYSTEM_NAME_LINUX})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
set_default (LUEPP_PLATFORM_TYPE ${LUEPP_PLATFORM_TYPE_DESKTOP})
set_default (LUEPP_SYSTEM_NAME   ${LUEPP_SYSTEM_NAME_WINDOWS})
# TODO: Other platforms like windows and osx
endif ()

# message ("LUEPP_PLATFORM_TYPEID: '${LUEPP_PLATFORM_TYPEID}'")
# message ("LUEPP_PLATFORM_NAMEID: '${LUEPP_PLATFORM_NAMEID}'")

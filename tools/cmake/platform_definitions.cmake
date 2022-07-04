# This file defines platform names, #defines and IDs etc. as well as setting
# default values for those values.
# IMPORTANT: Must match the definitions set in [luepp/]include/config/luepp_platform_definitions.h
#            The reason for the redundancy is to make it easier for users that do not want
#            to compile this using cmake!

# ----------------------------------
# --- Platform type id #defines  ---
# ----------------------------------

set (LUEPP_PLATFORM_TYPE_ID_DESKTOP      1   )
set (LUEPP_PLATFORM_TYPE_ID_EMBEDDED     2   )
set (LUEPP_PLATFORM_TYPE_ID_WEB          3   )
set (LUEPP_PLATFORM_TYPE_ID_MOBILE       4   )

# ---------------------------------------------
# --- System/abstraction layer id #defines  ---
# ---------------------------------------------
set (LUEPP_SYSTEM_ID_LINUX               1   )
set (LUEPP_SYSTEM_ID_WINDOWS             2   )
set (LUEPP_SYSTEM_ID_OSX                 3   )
set (LUEPP_SYSTEM_ID_FREERTOS            4   )
set (LUEPP_SYSTEM_ID_BROWSER             5   )
set (LUEPP_SYSTEM_ID_ANDROID             6   )
set (LUEPP_SYSTEM_ID_IOS                 7   )

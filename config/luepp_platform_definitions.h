#pragma once

// IMPORTANT: Must match the definitions set in [luepp/]tools/cmake/platform_definitions.cmake
//            The reason for the redundancy is to make it easier for users that do not want
//            to compile this using cmake!

#define LUEPP_PLATFORM_TYPE_ID_DESKTOP	 1
#define LUEPP_PLATFORM_TYPE_ID_EMBEDDED  2
#define LUEPP_PLATFORM_TYPE_ID_WEB       3
#define LUEPP_PLATFORM_TYPE_ID_MOBILE    4

#define LUEPP_SYSTEM_ID_LINUX			1
#define LUEPP_SYSTEM_ID_WINDOWS			2
#define LUEPP_SYSTEM_ID_OSX				3
#define LUEPP_SYSTEM_ID_FREERTOS		4
#define LUEPP_SYSTEM_ID_BROWSER			5
#define LUEPP_SYSTEM_ID_ANDROID			6
#define LUEPP_SYSTEM_ID_IOS				7


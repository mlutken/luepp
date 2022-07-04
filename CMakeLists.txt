cmake_minimum_required(VERSION 3.5)
project(luepp)
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/std_utils.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/default_versions.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/platform_definitions.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/default_platform_definitions.cmake)
#include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/set_platform_compiler_defines.cmake)

include("tools/cmake/HunterGate.cmake")

option(build_fsm "Build the FSM (Finite State Machine. Needs Boost)" OFF)

if (build_fsm)
    set (BOOST_MIN_VERSION 1.55)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
endif()


enable_testing()


set(ESTL_CXX_STANDARD_LIBRARY 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_definitions(
    -DLUEPP_PLATFORM_TYPE=${LUEPP_PLATFORM_TYPE}
    -DLUEPP_SYSTEM_NAME=${LUEPP_SYSTEM_NAME}
)

#set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wsign-conversion -Wno-zero-as-null-pointer-constant -Werror")
set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wsign-conversion -Werror")
set(C_COMPILE_FLAGS "-Wall -Wextra")
set(CXX_COMPILE_FLAGS "-Wall -Wextra")
string(REPLACE " " ";" C_COMPILE_FLAGS_ALL "${COMMON_COMPILE_FLAGS} ${C_COMPILE_FLAGS}")
string(REPLACE " " ";" CXX_COMPILE_FLAGS_ALL "${COMMON_COMPILE_FLAGS} ${CXX_COMPILE_FLAGS}")

# --- Set default platform values ---
#if (NOT DEFINED PLATFORM_NAME)
#    #set(PLATFORM_TYPE hostwindows)
#    set(PLATFORM_NAME hostlinux)
#endif()

#set_default(PLATFORM_NAME "hostlinux")

#message ("PLATFORM_NAME     : '${PLATFORM_NAME}'")
#message ("PRODUCTION_RELEASE: '${PRODUCTION_RELEASE}'")


# --- Add testing before setting general compiler flags ---
add_subdirectory(testing)

# Set general project wide compiler flags
add_compile_options(
    "$<$<STREQUAL:${PRODUCTION_RELEASE},1>:-DPRODUCTION_RELEASE>"
    "$<$<COMPILE_LANGUAGE:C>:${C_COMPILE_FLAGS_ALL}>"
    "$<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS_ALL}>"
)

# --- Add subdirectories ---
add_subdirectory(config)
add_subdirectory(estl)

# --- Add example files ---
#add_subdirectory(doc/examples)

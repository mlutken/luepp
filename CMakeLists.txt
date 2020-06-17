cmake_minimum_required(VERSION 3.5)
project(estlmain)
enable_testing()
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/default_versions.cmake)

set(ESTL_CXX_STANDARD_LIBRARY 98)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_definitions(
    -DCXX_STANDARD_LIBRARY=${ESTL_CXX_STANDARD_LIBRARY}
    -DCXX_STANDARD=${CMAKE_CXX_STANDARD}
    -DC_STANDARD=${CMAKE_C_STANDARD}
    -DNESTLE_PLATFORM_TYPE=${NESTLE_PLATFORM_TYPE}
    -DNESTLE_SYSTEM_NAME=${NESTLE_SYSTEM_NAME}
)



#set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wsign-conversion -Wno-zero-as-null-pointer-constant -Werror")
set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wsign-conversion -Werror")
set(C_COMPILE_FLAGS "-Wall -Wextra")
set(CXX_COMPILE_FLAGS "-Wall -Wextra")
string(REPLACE " " ";" C_COMPILE_FLAGS_ALL "${COMMON_COMPILE_FLAGS} ${C_COMPILE_FLAGS}")
string(REPLACE " " ";" CXX_COMPILE_FLAGS_ALL "${COMMON_COMPILE_FLAGS} ${CXX_COMPILE_FLAGS}")

# --- Set default platform values ---
if (NOT DEFINED PLATFORM_NAME)
    #set(PLATFORM_TYPE hostwindows)
    set(PLATFORM_NAME hostlinux)
endif()

message ("PLATFORM_NAME     : '${PLATFORM_NAME}'")
message ("PRODUCTION_RELEASE: '${PRODUCTION_RELEASE}'")

# --- Include cmake utility files ---
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/std_utils.cmake)

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
add_subdirectory(cesl)
add_subdirectory(estl)

# --- Add example files ---
#add_subdirectory(doc/examples)

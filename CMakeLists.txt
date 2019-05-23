cmake_minimum_required(VERSION 3.5)
project(estlmain)
enable_testing()
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/default_versions.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_definitions(
    -DPLATFORM_${PLATFORM}
    -DPLATFORM_NAME_${PLATFORM_NAME}
    -DPLATFORM_TYPE_${PLATFORM_TYPE}
    )

IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(
        -DLINUX=1
        )
ENDIF ()


set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wsign-conversion -Wno-zero-as-null-pointer-constant -Werror")
set(C_COMPILE_FLAGS "-Wall -Wextra")
set(CXX_COMPILE_FLAGS "-Wall -Wextra")
string(REPLACE " " ";" C_COMPILE_FLAGS_ALL "${COMMON_COMPILE_FLAGS} ${C_COMPILE_FLAGS}")
string(REPLACE " " ";" CXX_COMPILE_FLAGS_ALL "${COMMON_COMPILE_FLAGS} ${CXX_COMPILE_FLAGS}")

# --- Set default platform values ---
if (NOT DEFINED PLATFORM_NAME)
    set(PLATFORM native)   # Backwards compatibility ?
    set(PLATFORM_TYPE hostpc)  # PC: Windows, Linux, Mac etc.
    set(PLATFORM_NAME hostlinux)
    set(PLATFORM_LONG_NAME hostlinux_native)
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
add_subdirectory(estl)

# --- Add example files ---
#add_subdirectory(doc/examples)

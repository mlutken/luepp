cmake_minimum_required(VERSION 3.5)
project(luepp)
message ("(luepp)CMAKE_PROJECT_NAME      : '${CMAKE_PROJECT_NAME}'")

set (LUEPP_IS_TOP_LEVEL_PROJECT FALSE)
if ( "${CMAKE_PROJECT_NAME}" MATCHES "luepp")
    set (LUEPP_IS_TOP_LEVEL_PROJECT TRUE)
endif ()

SET(QT_CREATOR_SKIP_PACKAGE_MANAGER_SETUP TRUE)
SET(QT_CREATOR_SKIP_CONAN_SETUP TRUE)


include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/std_utils.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/platform_definitions.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/default_platform_definitions.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/set_platform_compiler_defines.cmake)

if (LUEPP_IS_TOP_LEVEL_PROJECT)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
      file(COPY_FILE "${CMAKE_CURRENT_LIST_DIR}/3rdparty/conan/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
    #  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake" TLS_VERIFY ON)
    endif()


    include(${CMAKE_BINARY_DIR}/conan.cmake)

    conan_cmake_autodetect(settings)

    conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_CURRENT_LIST_DIR}
                        BUILD missing
                        REMOTE conancenter
                        SETTINGS ${settings})

    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()



#include("tools/cmake/HunterGate.cmake")

option(build_fsm "Build the FSM (Finite State Machine. Needs Boost)" OFF)

if (build_fsm)
    set (BOOST_MIN_VERSION 1.55)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
endif()

if (LUEPP_IS_TOP_LEVEL_PROJECT)
    message ("INFO: luepp is top level project!")
    enable_testing()
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wsign-conversion -Wno-zero-as-null-pointer-constant -Werror")
set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wsign-conversion -Werror")
set(CXX_COMPILE_FLAGS "-Wall -Wextra")
string(REPLACE " " ";" CXX_COMPILE_FLAGS_ALL "${COMMON_COMPILE_FLAGS} ${CXX_COMPILE_FLAGS}")


# --- Add testing before setting general compiler flags ---
if (LUEPP_IS_TOP_LEVEL_PROJECT)
#    add_subdirectory(testing)
endif()

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

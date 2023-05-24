cmake_minimum_required(VERSION 3.22)
project(luepp)
message ("(luepp)CMAKE_PROJECT_NAME      : '${CMAKE_PROJECT_NAME}'")

# Emscripten CMake CMD: .../_build/luepp$ cmake  -DCMAKE_SYSTEM_NAME=Emscripten -DCMAKE_BUILD_TYPE=Release -DCONAN_PROFILE=emscripten -DCMAKE_C_COMPILER=/opt/user/emsdk/upstream/emscripten/emcc -DCMAKE_CXX_COMPILER=/opt/user/emsdk/upstream/emscripten/em++ -DCMAKE_AR=/opt/user/emsdk/upstream/emscripten/emar -DCMAKE_RANLIB=/opt/user/emsdk/upstream/emscripten/emranlib ../../luepp

set (LUEPP_IS_TOP_LEVEL_PROJECT FALSE)
if ( "${CMAKE_PROJECT_NAME}" MATCHES "luepp")
    set (LUEPP_IS_TOP_LEVEL_PROJECT TRUE)
endif ()

if (CMAKE_CROSSCOMPILING)
    message("INFO: CMAKE_CROSSCOMPILING for '${CMAKE_SYSTEM_NAME}'")
else()
    message("INFO: Compiling for current system '${CMAKE_SYSTEM_NAME}' ")
endif()


if (LUEPP_IS_TOP_LEVEL_PROJECT)
    include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/conan_utils.cmake)
    SET(QT_CREATOR_SKIP_PACKAGE_MANAGER_SETUP TRUE)
    SET(QT_CREATOR_SKIP_CONAN_SETUP TRUE)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/bin)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/bin)
    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/lib)
    set (CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/pdb)

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
        set (CMAKE_EXECUTABLE_SUFFIX    ".html" )
    endif ()

    if (NOT CMAKE_TOOLCHAIN_FILE)
        conan_utils_get_profile_settings(conan_profile_settings)
        conan_utils_get_profile_name(conan_profile)

        message ("INFO! Using Conan profile : ${conan_profile}")
        message ("INFO! Using Conan settings: ${conan_profile_settings}")

        conan_utils_cmake_install(PATH_OR_REFERENCE ${CMAKE_CURRENT_LIST_DIR}
                                  BUILD missing
                                  REMOTE conancenter
                                  OUTPUT_FOLDER ${CMAKE_BINARY_DIR}
                                  PROFILE ${conan_profile}
                                  SETTINGS ${conan_profile_settings})

        include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
    endif()
    find_package(Threads REQUIRED)
    if (NOT CMAKE_CROSSCOMPILING)
        message("INFO: Find GTest as we are NOT cross compiling!")
        find_package(GTest REQUIRED)
    endif()
endif()

include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/std_utils.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/platform_definitions.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/default_platform_definitions.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/set_platform_compiler_defines.cmake)

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
# /home/ml/.conan2/p/b/gtest5021f5ad01ab6/p/include
# /home/ml/.conan2/p/b/gtest1ffb7d05809f8/p/include


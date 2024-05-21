cmake_minimum_required(VERSION 3.22)
project(luepp)
message ("(luepp)CMAKE_PROJECT_NAME      : '${CMAKE_PROJECT_NAME}'")
set(CMAKE_CXX_STANDARD 23)

include(cmake/PreventInSourceBuilds.cmake)
include(ProjectOptions.cmake)


myproject_setup_options()

myproject_global_options()
include(Dependencies.cmake)
myproject_setup_dependencies()

myproject_local_options()


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
    find_package(Threads REQUIRED)
    if (NOT CMAKE_CROSSCOMPILING)
			CPMAddPackage(
				NAME googletest
				GITHUB_REPOSITORY google/googletest
				GIT_TAG v1.14.0
				VERSION v1.14.0
				OPTIONS
						"INSTALL_GTEST OFF"
						"gtest_force_shared_crt ON"
			)

			message("INFO: Find GTest as we are NOT cross compiling!")
			find_package(GTest REQUIRED)
    endif()
endif()

include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/std_utils.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/platform_definitions.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/default_platform_definitions.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/set_platform_compiler_defines.cmake)

option(build_fsm "Build the FSM (Finite State Machine. Needs Boost)" OFF)


if (LUEPP_IS_TOP_LEVEL_PROJECT)
    message ("INFO: luepp is top level project!")
    enable_testing()
endif()


set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wsign-conversion -Wno-zero-as-null-pointer-constant -Werror")
set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wsign-conversion -Werror")
set(CXX_COMPILE_FLAGS "-Wall -Wextra")
string(REPLACE " " ";" CXX_COMPILE_FLAGS_ALL "${COMMON_COMPILE_FLAGS} ${CXX_COMPILE_FLAGS}")

# --- Add testing before setting general compiler flags ---

# Set general project wide compiler flags
add_compile_options(
    "$<$<STREQUAL:${PRODUCTION_RELEASE},1>:-DPRODUCTION_RELEASE>"
    "$<$<COMPILE_LANGUAGE:C>:${C_COMPILE_FLAGS_ALL}>"
    "$<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS_ALL}>"
)

# --- Add subdirectories ---
add_subdirectory(config)
add_subdirectory(lue)


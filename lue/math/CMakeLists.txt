set (library_name lue_math)
add_library(${library_name} INTERFACE)

target_sources(${library_name}
    INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/base/lue_math_internal_utils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/base/constants.hpp
    ${CMAKE_CURRENT_LIST_DIR}/base/g33.hpp
    ${CMAKE_CURRENT_LIST_DIR}/base/g44.hpp
    ${CMAKE_CURRENT_LIST_DIR}/base/lue_math.h
    ${CMAKE_CURRENT_LIST_DIR}/base/m22.hpp
    ${CMAKE_CURRENT_LIST_DIR}/base/m33.hpp
    ${CMAKE_CURRENT_LIST_DIR}/base/m44.hpp
    ${CMAKE_CURRENT_LIST_DIR}/base/oct.hpp
    ${CMAKE_CURRENT_LIST_DIR}/base/quat.hpp
    ${CMAKE_CURRENT_LIST_DIR}/base/stdmath.hpp
    ${CMAKE_CURRENT_LIST_DIR}/base/utils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/base/v2.hpp
    ${CMAKE_CURRENT_LIST_DIR}/base/v3.hpp
    ${CMAKE_CURRENT_LIST_DIR}/base/v4.hpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
# get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_include_directories(${library_name} INTERFACE ${PARENT_DIR})

#     target_compile_options(${library_name} INTERFACE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()



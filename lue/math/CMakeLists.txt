set (library_name lue_math)
add_library(${library_name} INTERFACE)

target_sources(${library_name}
    INTERFACE
    base/lue_math_internal_utils.hpp
    base/constants.hpp
    base/g33.hpp
    base/g44.hpp
    base/lue_math.h
    base/m22.hpp
    base/m33.hpp
    base/m44.hpp
    base/oct.hpp
    base/quat.hpp
    base/stdmath.hpp
    base/utils.hpp
    base/v2.hpp
    base/v3.hpp
    base/v4.hpp

    primitives2d/rectangle.hpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
# get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_include_directories(${library_name} INTERFACE ${PARENT_DIR})

#     target_compile_options(${library_name} INTERFACE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()



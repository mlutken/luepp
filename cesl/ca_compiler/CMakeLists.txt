set (lib_name ca_compiler)

add_library(${lib_name} INTERFACE)

target_sources(${lib_name}
    INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/ca_align_macros.h
    ${CMAKE_CURRENT_LIST_DIR}/ca_atomic.h
    ${CMAKE_CURRENT_LIST_DIR}/ca_extern_c.h
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_include_directories(${lib_name} INTERFACE ${PARENT_DIR})

add_subdirectories_standard()

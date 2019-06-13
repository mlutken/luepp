set (lib_name ca_concurrent)

add_library(${lib_name} STATIC "")

target_sources(${lib_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/ca_llfifo.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ca_llfifo.c
)

target_link_libraries(${lib_name} ca_compiler)
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_include_directories(${lib_name} INTERFACE ${PARENT_DIR})

add_subdirectories_standard()

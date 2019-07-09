set (lib_name cesl_concurrent)

add_library(${lib_name} STATIC "")

target_sources(${lib_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cesl_llfifo.h
    ${CMAKE_CURRENT_LIST_DIR}/cesl_mpsc_llfifo.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cesl_llfifo.c
    ${CMAKE_CURRENT_LIST_DIR}/cesl_mpsc_llfifo.c
)

target_link_libraries(${lib_name} cesl_compiler)
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_include_directories(${lib_name} INTERFACE ${PARENT_DIR})

add_subdirectories_standard()

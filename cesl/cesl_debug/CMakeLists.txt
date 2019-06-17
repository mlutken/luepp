set (library_name cesl_debug)

add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cesl_debug.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform_${PLATFORM_NAME}/cesl_debug__${PLATFORM_NAME}.c
)

target_link_libraries(${library_name} cesl_compiler)
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_include_directories(${library_name} INTERFACE ${PARENT_DIR})

add_subdirectories_standard()

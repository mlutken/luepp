set (library_name my_dependant_module)

add_library(${library_name} STATIC "")

target_sources(${library_name}
    INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/my_dependant_module_mock.h
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/my_dependant_module.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/my_dependant_module.c
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_link_libraries(${library_name} my_base_module cesl_compiler cesl_gmock m)
target_include_directories(${library_name} INTERFACE ${PARENT_DIR})

add_subdirectories_standard()

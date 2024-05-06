set (library_name luepp_asic)

add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/commands.h
    ${CMAKE_CURRENT_LIST_DIR}/command_queue.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/commands.cpp
    ${CMAKE_CURRENT_LIST_DIR}/command_queue.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_link_libraries(${library_name} "luepp_config")
target_link_libraries(${library_name} "luepp_concurrent")
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} PUBLIC ${PARENT_DIR})

add_subdirectories_standard()

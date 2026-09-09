set (library_name luepp_util)

add_library(${library_name} STATIC ""
    data_concepts.h
    uri.h uri.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} PUBLIC ${PARENT_DIR})

add_subdirectories_standard()

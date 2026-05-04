set (library_name luepp_data)

add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    data_object_base.h
    data_path.h
    data_source.h
    data_value.h
    PRIVATE
    data_path.cpp
    data_source.cpp
    data_value.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_link_libraries(${library_name} PUBLIC "luepp_config")
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} PUBLIC ${PARENT_DIR})

add_subdirectories_standard()

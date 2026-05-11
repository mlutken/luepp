set (library_name luepp_data)

add_library(${library_name} STATIC 
    data_object_base.h
    data_path.h data_path.cpp
    data_source_base.h data_source_base.cpp
    data_source_memory.h data_source_memory.cpp
    data_value.h data_value.cpp
)


get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_link_libraries(${library_name} PUBLIC "luepp_config")
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} PUBLIC ${PARENT_DIR})

add_subdirectories_standard()

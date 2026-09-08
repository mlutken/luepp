set (library_name luepp_gui_common)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(${library_name} STATIC ""
    app.h app.cpp
    # platform_qt6/app_impl.h platform_qt6/app_impl.cpp
)


get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT_DIR2 ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} PUBLIC "luepp_system")
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} PUBLIC ${PARENT_DIR2})

add_subdirectories_standard()

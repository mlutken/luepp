set (library_name luepp_system)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(${library_name} STATIC ""
    core_app.h core_app.cpp
    app.h app.cpp

    platform_qt6/app_impl.h platform_qt6/app_impl.cpp

)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_link_libraries(${library_name} PUBLIC  Qt6::Widgets)
target_link_libraries(${library_name} PUBLIC "luepp_config")
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} PUBLIC ${PARENT_DIR})

add_subdirectories_standard()

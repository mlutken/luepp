set (library_name luepp_gui_widgets)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(${library_name} STATIC ""
    # app.h app.cpp
    # container.h container.cpp
    label.h label.cpp
    # main_window.h main_window.cpp
    widget_base.h widget_base.cpp


    # platform_qt6/app_impl.h platform_qt6/app_impl.cpp
    platform_qt6/label_impl.h platform_qt6/label_impl.cpp
    # platform_qt6/main_window_impl.h platform_qt6/main_window_impl.cpp
    platform_qt6/widget_base_impl.h platform_qt6/widget_base_impl.cpp
    layoutable.h layoutable.cpp
    # platform_qt6/widget_impl.h platform_qt6/widget_impl.cpp
    # platform_qt6/qt_utils.h platform_qt6/qt_utils.cpp
)


get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_link_libraries(${library_name} PUBLIC "luepp_system")
target_link_libraries(${library_name} PUBLIC  Qt6::Widgets)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} PUBLIC ${PARENT_DIR})

add_subdirectories_standard()

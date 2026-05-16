set (library_name luepp_gui)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(${library_name} STATIC ""
    app.h app.cpp
    label.h label.cpp
    widget_base.h widget_base.cpp
    sandbox/page_type_a.h sandbox/page_type_a.cpp
    sandbox/test_page_a1.h sandbox/test_page_a1.cpp
    container.h container.cpp
    qt/app_impl.h qt/app_impl.cpp
    qt/container_impl.h qt/container_impl.cpp
    qt/label_impl.h qt/label_impl.cpp
    qt/widget_impl.h qt/widget_impl.cpp
    qt/qt_utils.h qt/qt_utils.cpp
)


get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_link_libraries(${library_name} PUBLIC "luepp_system")
target_link_libraries(${library_name} PUBLIC "luepp_data")
target_link_libraries(${library_name} PUBLIC  Qt6::Widgets)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} PUBLIC ${PARENT_DIR})

add_subdirectories_standard()

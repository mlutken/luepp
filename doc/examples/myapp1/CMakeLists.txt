add_executable(myapp1 "")
target_link_libraries(myapp1 foolib)
target_sources(myapp1
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/myapp1.c"
)

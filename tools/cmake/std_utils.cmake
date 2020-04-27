# ---------------------------
# --- Directory functions ---
# ---------------------------

# Get immediate parent directory
# @sa parent_directory2
function(parent_directory1 dir_out)
    get_filename_component(PARENT_DIR1 ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
    set (${dir_out} ${PARENT_DIR1} PARENT_SCOPE)
endfunction()

# Get 2 levels up parent directory
# @sa parent_directory1
function(parent_directory2 dir_out)
    get_filename_component(PARENT_DIR1 ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
    get_filename_component(PARENT_DIR2 ${PARENT_DIR1} DIRECTORY)
    set (${dir_out} ${PARENT_DIR2} PARENT_SCOPE)
endfunction()

# --------------------------------------------
# --- Target include directories functions ---
# --------------------------------------------

# --------------------------------
# --- Subdirectories functions ---
# --------------------------------
function(add_subdirectories_standard)
    if (NOT CMAKE_CROSSCOMPILING AND NOT ("${CMAKE_CXX_STANDARD}" STREQUAL "98") AND EXISTS ${CMAKE_CURRENT_LIST_DIR}/unittest)
        add_subdirectory(unittest)
    endif()
    if (NOT CMAKE_CROSSCOMPILING AND NOT ("${CMAKE_CXX_STANDARD}" STREQUAL "98") AND EXISTS ${CMAKE_CURRENT_LIST_DIR}/integrationtest)
        add_subdirectory(integrationtest)
    endif()
    if (NOT ("${CMAKE_CXX_STANDARD}" STREQUAL "98") AND EXISTS ${CMAKE_CURRENT_LIST_DIR}/playground)
        add_subdirectory(playground)
    endif()
endfunction()

function(add_subdirectories_standard_98)
    if (NOT CMAKE_CROSSCOMPILING AND NOT ("${CMAKE_CXX_STANDARD}" STREQUAL "98") AND EXISTS ${CMAKE_CURRENT_LIST_DIR}/unittest)
        add_subdirectory(unittest)
    endif()
    if (NOT CMAKE_CROSSCOMPILING AND NOT ("${CMAKE_CXX_STANDARD}" STREQUAL "98") AND EXISTS ${CMAKE_CURRENT_LIST_DIR}/integrationtest)
        add_subdirectory(integrationtest)
    endif()
    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/playground)
        add_subdirectory(playground)
    endif()
endfunction()

# -----------------------------
# --- Add simple executable ---
# -----------------------------
function(add_cpp_exe exe_name link_libs)
    add_executable(${exe_name} "")
    target_link_libraries(${exe_name} ${link_libs})
    target_sources(${exe_name}
        PRIVATE
            "${CMAKE_CURRENT_LIST_DIR}/${exe_name}.cpp"
    )
endfunction()

# ---------------------------
# --- Unit test functions ---
# ---------------------------
function(add_unit_test name link_libs)
    if (NOT CMAKE_CROSSCOMPILING)
        set (test_name  ${name}_unittest)
        set (test_link_libraries ${link_libs})
        find_package(Threads REQUIRED)

        add_executable(${test_name} "")
        target_sources(${test_name}
            PRIVATE
                "${CMAKE_CURRENT_LIST_DIR}/${test_name}.cpp"
        )

        target_link_libraries(${test_name} ${test_link_libraries} gmock gtest_main gtest ${CMAKE_THREAD_LIBS_INIT})

        add_test(
            NAME ${test_name}
            COMMAND ${test_name}
        )
    endif()
endfunction()

# ----------------------------------
# --- Integration test functions ---
# ----------------------------------
function(add_integration_test name link_libs)
    if (NOT CMAKE_CROSSCOMPILING)
        set (test_name  ${name}_integrationtest)
        set (test_link_libraries ${link_libs})
        find_package(Threads REQUIRED)

        add_executable(${test_name} "")
        target_sources(${test_name}
            PRIVATE
                "${CMAKE_CURRENT_LIST_DIR}/${test_name}.cpp"
        )

        target_link_libraries(${test_name} ${test_link_libraries} gmock gtest_main gtest ${CMAKE_THREAD_LIBS_INIT})

        add_test(
            NAME ${test_name}
            COMMAND ${test_name}
        )
    endif()
endfunction()

# ----------------------------
# --- Playground functions ---
# ----------------------------
function(add_playground_c_exe name link_libs)
    set (playground_name  ${name}_playground)

    add_executable(${playground_name} "")
    target_link_libraries(${playground_name} ${link_libs})
    target_sources(${playground_name}
        PRIVATE
            "${CMAKE_CURRENT_LIST_DIR}/${playground_name}.c"
    )
endfunction()

function(add_playground_cpp_exe name link_libs)
    set (playground_name  ${name}_playground)

    add_executable(${playground_name} "")
    target_link_libraries(${playground_name} ${link_libs})
    target_sources(${playground_name}
        PRIVATE
            "${CMAKE_CURRENT_LIST_DIR}/${playground_name}.cpp"
    )
endfunction()

if (NOT ("${CMAKE_CXX_STANDARD}" STREQUAL "98"))
    add_cpp_exe(fsm2dot "fsm;boost_filesystem;boost_system;boost_program_options")
endif()

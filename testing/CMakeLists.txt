
# GTest cannot compile with all warning set as errors :(
set(COMMON_COMPILE_FLAGS "-Wall -Wextra")
set(C_COMPILE_FLAGS "-Wall -Wextra")
set(CXX_COMPILE_FLAGS "-Wall -Wextra")
string(REPLACE " " ";" C_COMPILE_FLAGS_ALL "${COMMON_COMPILE_FLAGS} ${C_COMPILE_FLAGS}")
string(REPLACE " " ";" CXX_COMPILE_FLAGS_ALL "${COMMON_COMPILE_FLAGS} ${CXX_COMPILE_FLAGS}")

add_compile_options(
    "$<$<STREQUAL:${PRODUCTION_RELEASE},1>:-DPRODUCTION_RELEASE>"
    "$<$<COMPILE_LANGUAGE:C>:${C_COMPILE_FLAGS_ALL}>"
    "$<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS_ALL}>"
)

if (NOT CMAKE_CROSSCOMPILING)
    add_subdirectory( googletest )
endif()

#add_subdirectory( cas_gmock )

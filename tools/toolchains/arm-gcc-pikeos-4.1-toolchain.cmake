set(PIKEOS_VERSION 4.1)
set(PIKEOS_GCC_VERSION 5.3.0)

set(PLATFORM pikeos)   # Backwards compatibility ?
set(PLATFORM_TYPE embedded)  # As opposed to you Linux/Windows development PC
set(PLATFORM_NAME pikeos)
set(PLATFORM_LONG_NAME pikeos_native)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(PIKEOS_PREFIX /opt/pikeos-${PIKEOS_VERSION})
set(PIKEOS_TARGET_FILES ${PIKEOS_PREFIX}/target/arm/v7hf)
set(PIKEOS_CENV_LINK_DIR ${PIKEOS_PREFIX}/target/arm/v7hf/cenv/lib)
set(PIKEOS_LINK_DIR ${PIKEOS_PREFIX}/target/arm/v7hf/lib)
set(PIKEOS_TOOLS_BIN_DIR ${PIKEOS_PREFIX}/cdk/arm/v7hf/bin)

set(CMAKE_SYSROOT ${PIKEOS_PREFIX}/cdk/arm/v7hf)
#set(CMAKE_STAGING_PREFIX /home/developer/stage)

set(CMAKE_C_COMPILER ${PIKEOS_TOOLS_BIN_DIR}/arm_v7hf-gcc)
set(CMAKE_CXX_COMPILER ${PIKEOS_TOOLS_BIN_DIR}/arm_v7hf-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

include_directories(
    ${PIKEOS_TARGET_FILES}/include
    ${PIKEOS_TARGET_FILES}/driver/include
    ${PIKEOS_TARGET_FILES}/pikeos-native/include
    ${PIKEOS_TARGET_FILES}/pikeos-native/include/p4ext
    ${PIKEOS_TARGET_FILES}/cenv/include
    )

add_definitions(
    -DPIKEOSCENV
    -DARM_V7HF
    )


# INFO: For reference see default implementation of CMAKE_C_LINK_EXECUTABLE in
#       /usr/share/cmake-VERSION/modules/CMakeCInformation.cmake
set(CMAKE_C_LINK_EXECUTABLE "${PIKEOS_TOOLS_BIN_DIR}/arm_v7hf-ld -L${PIKEOS_TARGET_FILES}/pikeos-native/lib -L${PIKEOS_TARGET_FILES}/lib -L${PIKEOS_PREFIX}/cdk/arm/v7hf/lib/gcc/arm-unknown-eabihf/${PIKEOS_GCC_VERSION} -L${PIKEOS_CENV_LINK_DIR} <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> -T${PIKEOS_TARGET_FILES}/scripts/app-ld-script -o <TARGET> <OBJECTS> <LINK_LIBRARIES> -lp4ext -lvm -lp4 -lstand -lgcc")

set (CMAKE_C_FLAGS "-Wsuggest-attribute=format" CACHE STRING "" FORCE )

# Other options if needed!
# Probably not needed when we define CMAKE_C_LINK_EXECUTABLE
# link_directories(
#     /example/link/dir
#    )
# set (
#     CMAKE_STANDARD_LIBRARIES
#     /mydir/libfoo.a
#     )

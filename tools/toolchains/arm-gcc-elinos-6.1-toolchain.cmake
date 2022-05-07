set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(PLATFORM elinos)   # Backwards compatibility ?
set(PLATFORM_TYPE embedded)  # As opposed to you Linux/Windows development PC
set(PLATFORM_NAME elinos)
set(PLATFORM_LONG_NAME elinos_native)

set(CMAKE_SYSROOT /opt/elinos-6.1/cdk/arm/v7hf/glibc-2.19/arm-unknown-linux-gnueabihf)
set(CMAKE_STAGING_PREFIX /home/developer/stage)

set(tools /opt/elinos-6.1/cdk/arm/v7hf/glibc-2.19 )
set(CMAKE_C_COMPILER ${tools}/bin/arm-unknown-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm-unknown-linux-gnueabihf-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

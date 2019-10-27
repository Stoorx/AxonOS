cmake_minimum_required(VERSION 3.12)

# Includes
project(AxonOS)
add_definitions(-D__AXON__)

set(CMAKE_CXX_STANDARD 17)

# Build config
include(BuildConfig.cmake)

# Enable Languages
enable_language(C)
enable_language(CXX)
enable_language(ASM_NASM)


# Util
include(sdk/cmake/util/compiler-flags.cmake)

# Versioning
include(sdk/cmake/version.cmake)

# Internationalization configure
# TODO

# Generic macros
# include(cmake/util/macro.cmake)
# TODO

# Include directories TODO
include_directories(common/include)
include_directories(${CMAKE_SOURCE_DIR})

# Add subdirectories TODO
add_subdirectory(Boot)
#add_subdirectory(Kernel)
add_subdirectory(Test)
add_subdirectory(tools)


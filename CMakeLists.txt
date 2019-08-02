# Includes
include(ProcessorCount)

# Global Config
cmake_minimum_required(VERSION 3.12)
project(AxonOS C CXX ASM)
include(BuildConfig.cmake)
set(CMAKE_CXX_STANDARD 17)

# Ad-hoc
set(CMAKE_CXX_FLAGS "-fpermissive")

# custom directoriest
set(CMAKE_TEMP_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/temp)
file(MAKE_DIRECTORY ${CMAKE_TEMP_OUTPUT_DIRECTORY})


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Directories
add_subdirectory(Boot)
add_subdirectory(Kernel)
add_subdirectory(Test)
add_subdirectory(Tools)
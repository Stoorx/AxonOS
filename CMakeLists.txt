cmake_minimum_required(VERSION 3.12)
project(AxonOS C CXX ASM)

include(BuildConfig.cmake)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Boot)
add_subdirectory(Kernel)
add_subdirectory(Test)
add_subdirectory(Tools)
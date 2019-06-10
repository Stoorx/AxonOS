cmake_minimum_required(VERSION 3.12)
project(AxonOS)

include(BuildConfig.cmake)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Kernel)
add_subdirectory(Test)
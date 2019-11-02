add_library(AeroFramework STATIC
        Object.cpp Object.hpp)

add_library(CRuntimeLibrary STATIC
        memset.c memset.h itoa.c itoa.h)

target_include_directories(CRuntimeLibrary PUBLIC ${CMAKE_SOURCE_DIR}/common/include)
target_include_directories(CRuntimeLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
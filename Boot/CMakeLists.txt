
include(${PROJECT_SOURCE_DIR}/sdk/cmake/cross-build.cmake)


if (${Arch} STREQUAL "amd64")
    add_subdirectory(x86)
endif ()
add_subdirectory(AxonBoot)


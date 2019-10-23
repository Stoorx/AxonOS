# SCOPE = AxonOS

if (Arch STREQUAL "amd64")
    include(${CMAKE_CURRENT_LIST_DIR}/config-amd64.cmake)
elseif (Arch STREQUAL "x86")
    include(${CMAKE_CURRENT_LIST_DIR}/config-x86.cmake)
endif ()

set(MultiProc TRUE CACHE BOOL
        "Build multiprocessor system.")

set(NumaSupport FALSE CACHE BOOL
        "Build system for NUMA platforms.")
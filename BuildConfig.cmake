# AxonOS main build config

set(Arch "amd64" CACHE STRING
        "Set target architecture: x86 amd64. ARM is not supported yet.")

set(CrossEnvPath "/usr/x86_64-pc-linux-gnu" CACHE STRING
        "Path to binutils for crosscompilling.")

set(DistDirectory "${CMAKE_CURRENT_SOURCE_DIR}/dist" CACHE STRING
        "Where is raw distribution directory.")

set(CMAKE_VERBOSE_MAKEFILE FALSE)

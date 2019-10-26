

set(KernelVersionMajor "0")
set(KernelVersionMinor "1")
set(KernelVersionPatch "0")

# "dev" for development
# "qa" for testing
# "rel" or ""for release
set(KernelBuildType "dev")


if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
    find_package(Git)
    if (GIT_FOUND)
        execute_process(
                COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE GitCommitHash
                RESULT_VARIABLE GitExitCode
                OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if (GitExitCode EQUAL 0)
            set(CommitHash "${GitCommitHash}")
            string(SUBSTRING ${CommitHash} 0 8 CommitHashShort)
        endif ()
    else ()
        message(FATAL_ERROR "Git package not found")
    endif ()
endif ()


if (NOT CommitHashShort STREQUAL "")
    set(KernelVersionClear "${KernelVersionMajor}.${KernelVersionMinor}.${KernelVersionPatch}.${CommitHashShort}")
    set(KernelVersion ${KernelVersionClear})
    if (NOT KernelBuildType STREQUAL "")
        set(KernelVersion "${KernelVersion}-${KernelBuildType}")
    endif ()
    configure_file(${CMAKE_CURRENT_LIST_DIR}/version.cmake.h ${CMAKE_SOURCE_DIR}/common/include/version.h)
    message("-- Kernel version: ${KernelVersion}")
else ()
    message(FATAL_ERROR "Cannot get commit hash")
endif ()

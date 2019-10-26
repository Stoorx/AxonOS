

add_executable(AxonBoot AxonBoot.cpp Util/Main.cpp)
AddLinkerScript(AxonBoot AxonBoot.lds)
set_property(TARGET AxonBoot APPEND PROPERTY OUTPUT_NAME AXONBOOT)

set(CMAKE_EXECUTABLE_SUFFIX "")
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
# Fix ASM_NASM compiler information bug
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -f ${CMAKE_ASM_NASM_OBJECT_FORMAT} -o <OBJECT> <SOURCE>")

AddCompileFlagsLang("-m32" C)
AddCompileFlagsLang("-m32" CXX)

target_include_directories(AxonBoot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(AxonBoot PUBLIC ${CMAKE_SOURCE_DIR}/common/include)

if (${Arch} STREQUAL "amd64")
    add_subdirectory(x86)
endif ()

add_subdirectory(lib) # MUST BE MOVED to the root of project
target_include_directories(AxonBoot PUBLIC lib)

AddFileToDist(AxonBoot AXONBOOT)
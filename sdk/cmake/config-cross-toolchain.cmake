set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_NAME AxonOS)
set(CMAKE_SYSTEM_VERSION ${KernelVersionClear})

# Specify the cross compiler
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang)

# where is the target environment
set(CMAKE_FIND_ROOT_PATH ${CrossEnvPath})
set(TargetTriple x86_64-pc-none-elf64)
set(TargetTripleLd x86_64-pc-linux-gnu)

if (${Arch} STREQUAL amd64)
    AddCompileFlagsLang("-target ${TargetTriple}" C)
    AddCompileFlagsLang("-target ${TargetTriple}" CXX)
endif ()

set(CMAKE_LINKER /usr/${TargetTripleLd}/bin/ld)
set(CMAKE_EXE_LINKER_FLAGS "")


add_definitions(-DNO_UNDERSCORE_PREFIX)

AddCompileFlagsLang(-nostdinc C)
AddCompileFlagsLang(-nostdinc CXX)

AddCompileFlagsLang(-std=c++17 CXX)
AddCompileFlagsLang(-std=c17 C)

set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")

AddCompileFlagsLang("-fno-rtti -fno-exceptions" CXX)

# TODO: add debugging

# Tune
AddCompileFlagsLang("-march=${TuneArch}" C)
AddCompileFlagsLang("-march=${TuneArch}" CXX)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_flags("-DNDEBUG")
endif ()

if (OPTIMIZE STREQUAL "1")
    AddCompileFlagsLang("-Os -ftracer" C)
    AddCompileFlagsLang("-Os -ftracer" CXX)
elseif (OPTIMIZE STREQUAL "2")
    AddCompileFlagsLang("-Os" C)
    AddCompileFlagsLang("-Os" CXX)
elseif (OPTIMIZE STREQUAL "3")
    AddCompileFlagsLang("-Og" C)
    AddCompileFlagsLang("-Og" CXX)
elseif (OPTIMIZE STREQUAL "4")
    AddCompileFlagsLang("-O1" C)
    AddCompileFlagsLang("-O1" CXX)
elseif (OPTIMIZE STREQUAL "5")
    AddCompileFlagsLang("-O2" C)
    AddCompileFlagsLang("-O2" CXX)
elseif (OPTIMIZE STREQUAL "6")
    AddCompileFlagsLang("-O3" C)
    AddCompileFlagsLang("-O3" CXX)
elseif (OPTIMIZE STREQUAL "7")
    AddCompileFlagsLang("-Ofast" C)
    AddCompileFlagsLang("-Ofast" CXX)
endif ()


set(CMAKE_C_LINK_EXECUTABLE
        "<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_CXX_LINK_EXECUTABLE
        "<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_C_CREATE_SHARED_LIBRARY
        "<CMAKE_LINKER> <CMAKE_SHARED_LIBRARY_C_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>"
        )
set(CMAKE_CXX_CREATE_SHARED_LIBRARY
        "<CMAKE_LINKER> <CMAKE_SHARED_LIBRARY_CXX_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>"
        )

set(CMAKE_C_COMPILE_OBJECT
        "<CMAKE_C_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")
set(CMAKE_CXX_COMPILE_OBJECT
        "<CMAKE_CXX_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")
#set(CMAKE_ASM_COMPILE_OBJECT
#        "<CMAKE_ASM_COMPILER> -o <OBJECT> <FLAGS> <SOURCE>")


function(CreateBootSectorTarget targetName asmFile)
    add_custom_command(
            OUTPUT ${targetName}-f
            COMMAND ${CMAKE_ASM_NASM_COMPILER} -o ${targetName} -f bin ${asmFile}
            DEPENDS ${asmFile})
    set_source_files_properties(${targetName} PROPERTIES GENERATED TRUE)
    add_custom_target(${targetName} ALL DEPENDS ${targetName}-f SOURCES ${asmFile})
endfunction()
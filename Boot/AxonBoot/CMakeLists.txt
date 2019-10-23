
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS} -f elf64)
add_executable(AxonBoot AxonBoot.cpp Util/Main.cpp)
AddLinkerScript(AxonBoot AxonBoot.lds)
set_property(TARGET AxonBoot APPEND PROPERTY OUTPUT_NAME AXONBOOT)
set(CMAKE_EXECUTABLE_SUFFIX "")
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_DEBUG "")
target_include_directories(AxonBoot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Print all variables
#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

if (${Arch} STREQUAL "amd64")
    add_subdirectory(x86)
endif ()
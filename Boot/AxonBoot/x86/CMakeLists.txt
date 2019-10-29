
set_property(TARGET AxonBoot APPEND PROPERTY SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Startup.asm
        ${CMAKE_CURRENT_SOURCE_DIR}/RealModeCall.asm)

set_property(TARGET AxonBoot APPEND PROPERTY SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/RealModeCall.h)
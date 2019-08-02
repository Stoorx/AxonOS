if (${TARGET_ARCHITECTURE} STREQUAL "x86")
    add_subdirectory(x86)
endif ()

add_custom_target(AxonBoot DEPENDS Main.cpp) # TODO: add Startup.asm dependency
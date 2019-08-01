add_custom_target(AxonBoot)
if (${TARGET_ARCHITECTURE} STREQUAL "x86")
    add_subdirectory(x86)
endif ()
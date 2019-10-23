get_property(targetArchitecture GLOBAL PROPERTY TargetArchitecture)
if (${targetArchitecture} STREQUAL "x86_64")
    add_subdirectory(x86)
endif ()
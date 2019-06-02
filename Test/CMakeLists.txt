include_directories(../Tools/Catch2)
include_directories(../Kernel/Include)

add_executable(GdtDescriptorTest Kernel/Memory/Arch/x86/GdtDescriptorTest.cpp)

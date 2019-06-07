include_directories(../Tools/Catch2)
include_directories(../Kernel/Include)
include_directories(../Util/Include)


add_executable(GdtSegmentDescriptorTest Kernel/Memory/Arch/x86/GdtSegmentDescriptorTest.cpp ../Tools/Catch2/CatchMain.cpp ../Kernel/Src/Memory/Arch/x86/GdtSegmentDescriptor.cpp)
add_executable(GdtWrapperTest Kernel/Memory/Arch/x86/GdtWrapperTest.cpp ../Tools/Catch2/CatchMain.cpp)
add_executable(IdtGateDescriptorTest32 Kernel/Memory/Arch/x86/IdtGateDescriptor32Test.cpp ../Tools/Catch2/CatchMain.cpp ../Kernel/Src/Memory/Arch/x86/IdtGateDescriptor32.cpp)

add_executable(AvlTreeTest Util/AvlTreeTest.cpp ../Util/Src/AvlTree.c ../Tools/Catch2/CatchMain.cpp)

add_executable(BuddyAllocatorTest Kernel/Memory/Allocators/BuddyAllocatorTest.cpp ../Tools/Catch2/CatchMain.cpp)
add_definitions(-DDEBUG)
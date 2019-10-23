include_directories(../tools/Catch2)
include_directories(../Kernel/Include)
include_directories(../Util/Include)

add_executable(GdtSegmentDescriptorTest Kernel/Memory/Arch/x86/GdtSegmentDescriptorTest.cpp ../tools/Catch2/CatchMain.cpp ../Kernel/Src/Memory/Arch/x86/GdtSegmentDescriptor.cpp)
add_executable(GdtWrapperTest Kernel/Memory/Arch/x86/GdtWrapperTest.cpp ../tools/Catch2/CatchMain.cpp)
add_executable(IdtGateDescriptorTest32 Kernel/Memory/Arch/x86/IdtGateDescriptor32Test.cpp ../tools/Catch2/CatchMain.cpp ../Kernel/Src/Memory/Arch/x86/IdtGateDescriptor32.cpp)

add_executable(AvlTreeTest Util/AvlTreeTest.cpp ../Util/Src/AvlTree.c ../tools/Catch2/CatchMain.cpp)
add_executable(StackAllocatorTest Kernel/Memory/Allocators/StackAllocatorTest.cpp ../Kernel/Src/Memory/Allocators/StackAllocator.c ../tools/Catch2/CatchMain.cpp)

//
// Created by Stoorx on 08.06.2019.
//

#include <Memory/Allocators/StackAllocator.h>
#include <Catch.hpp>

TEST_CASE("Regular allocation") {
    Word           size   = 4096;
    VoidPtr        region = (VoidPtr)0x1000;
    StackAllocator sa;
    StackAllocator$StackAllocator(&sa, region, size);
    
    VoidPtr allocated = StackAllocator$Allocate(&sa, 10);
    REQUIRE((Word)allocated == (Word)region);
    REQUIRE((Word)allocated + 10 == (Word)sa.CurrentPosition);
    REQUIRE(StackAllocator$BytesAllocated(&sa) == 10);
}

TEST_CASE("Aligned allocation") {
    Word           size   = 4096;
    VoidPtr        region = (VoidPtr)0x1237;
    StackAllocator sa;
    StackAllocator$StackAllocator(&sa, region, size);
    
    // Break alignment
    StackAllocator$Allocate(&sa, 15);
    
    VoidPtr alignedAlloc = StackAllocator$AllocateAligned(&sa, 1024, 256);
    REQUIRE(((Word)alignedAlloc & 0xFFu) == 0);
    REQUIRE(StackAllocator$BytesAllocated(&sa) == 1024 + 256 - ((Word)region & 0xFFu));
}

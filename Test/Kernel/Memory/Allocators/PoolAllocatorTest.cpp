//
// Created by Stoorx on 06.06.2019.
//

#include <Catch.hpp>
#include <Memory/Allocators/PoolAllocator.hpp>
#include <stdlib.h>

using Memory::Allocators::PoolAllocator;

class TestClass {
    ULong a;
    ULong b;
};

TEST_CASE("PoolChunk") {
    UInt                                size   = 56;
    VoidPtr                             region = malloc(size);
    PoolAllocator<TestClass>::PoolChunk pc((TestClass*)region, size);
    
    auto a1 = pc.Allocate();
    REQUIRE(a1 != NULL);
    
    auto a2 = pc.Allocate();
    REQUIRE(a2 != NULL);
    
    auto a3 = pc.Allocate();
    REQUIRE(a3 != NULL);
    
    auto a4 = pc.Allocate();
    REQUIRE(a4 == NULL);
    
    auto a5 = pc.Allocate();
    REQUIRE(a5 == NULL);
    
    REQUIRE(pc.Free(a5) == (ULong)PoolAllocator<TestClass>::PoolChunkFreeResult::NullPointerGiven);
    REQUIRE(pc.Free(a2) == (ULong)PoolAllocator<TestClass>::PoolChunkFreeResult::Success);
    REQUIRE(
        pc.Free((TestClass*)((ULong)a3 + 3)) == (ULong)PoolAllocator<TestClass>::PoolChunkFreeResult::InvalidPointer
    );
    
    return;
}

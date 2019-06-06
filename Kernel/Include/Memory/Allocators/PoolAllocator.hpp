//
// Created by Stoorx on 06.06.2019.
//

#pragma once

#include <Types.h>

namespace Memory::Allocators {
template <class T>
class PoolAllocator {
public:
    enum class PoolChunkFreeResult : ULong {
        Success         = 0,
        OutOfChunkBlock = 1,
        InvalidPointer  = 2,
        DoubleFree      = 3,
        NullPointerGiven = 4
    };
    
    class PoolChunk {
    public:
        PoolChunk(T* startAddress, ULong sizeInBytes) {
            StartAddress        = startAddress;
            TotalCount          = sizeInBytes / sizeof(T);
            AllocatedCount      = 0;
            Head                = StartAddress;
            TotalSizeInBytes    = sizeInBytes;
#ifdef DEBUG
            auto      debugFill = (BytePtr)startAddress;
            for (Long i         = 0; i < sizeInBytes; ++i) {
                *debugFill = 0xDE;
                debugFill++;
            }
#endif
            T* currentElement = StartAddress;
            for (Long i          = 0; i < TotalCount - 1; i++) {
                *(T**)currentElement = currentElement + 1;
                currentElement++;
            }
            *(T**)currentElement = nullptr;
        }
        
        T* Allocate(Bool zeroFill = false) {
            if (Head == nullptr) {
                return nullptr;
            }
            auto oldHead = Head;
            Head = *(T**)Head;
            AllocatedCount++;
            
            if (zeroFill) {
                for (Long i = 0; i < sizeof(T); i++) {
                    *(Byte*)oldHead = 0;
                }
            }
            return oldHead;
        }
        
        ULong Free(T* block) {
            if(block == nullptr)
                return (ULong)PoolChunkFreeResult::NullPointerGiven;
            
            if ((ULong)block >= (ULong)StartAddress + TotalSizeInBytes || (ULong)block < (ULong)StartAddress){
                return (ULong)PoolChunkFreeResult::OutOfChunkBlock;
            }
            // Check double free?
            if(((ULong)block - (ULong)StartAddress) % sizeof(T) != 0){
                return (ULong)PoolChunkFreeResult::InvalidPointer;
            }
            
            *(T**)block = Head;
            Head = block;
            AllocatedCount--;
            return (ULong)PoolChunkFreeResult::Success;
        }

#ifndef DEBUG
        protected:
#endif
        T* StartAddress;
        T* Head;
        ULong TotalCount;
        ULong AllocatedCount;
        ULong TotalSizeInBytes;
    };
};
}

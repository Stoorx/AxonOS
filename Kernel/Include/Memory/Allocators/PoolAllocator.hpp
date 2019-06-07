//
// Created by Stoorx on 06.06.2019.
//

#pragma once

#include <Types.h>
#include <AvlTree.h>

namespace Memory::Allocators {

template <class T>
class PoolAllocator {
public:
    class PoolChunk;
    
    PoolAllocator() {
        ChunksTree = {};
        RtlAvlTreeInitialize(
            &ChunksTree, [](PAVL_TREE_NODE first, PAVL_TREE_NODE second) -> Int {
                PoolChunk* firstChunk  = PARENT_STRUCTURE(first, PoolChunk, ThisChunkNode);
                PoolChunk* secondChunk = PARENT_STRUCTURE(second, PoolChunk, ThisChunkNode);
                
                // First inside second
                if ((ULong)firstChunk->StartAddress >= (ULong)secondChunk->StartAddress
                    && (ULong)firstChunk->StartAddress + firstChunk->TotalSizeInBytes <=
                       (ULong)secondChunk->StartAddress + secondChunk->TotalSizeInBytes) {
                    return RtlAvlTreeComparatorResult::Equals;
                }
                
                // Start of first is upper than end of second
                if ((ULong)firstChunk->StartAddress >
                    (ULong)secondChunk->StartAddress + secondChunk->TotalSizeInBytes) {
                    return RtlAvlTreeComparatorResult::Greater;
                }
                
                // End of first is lower than start of second
                if ((ULong)firstChunk->StartAddress + firstChunk->TotalSizeInBytes <
                    (ULong)secondChunk->StartAddress) {
                    return RtlAvlTreeComparatorResult::Less;
                }
                
                return RtlAvlTreeComparatorResult::ComparisionError;
            }
        );
        // TODO?
    }
    
    PoolChunk* SearchChunk(T* block) {
        PoolChunk testChunk = PoolChunk();
        testChunk.StartAddress     = block;
        testChunk.TotalSizeInBytes = 0;
        
        return PARENT_STRUCTURE(RtlAvlTreeSearch(&ChunksTree, &testChunk.ThisChunkNode), PoolChunk, ThisChunkNode);
    }
    
    T* Allocate() {
        // TODO
    }
    
    Int AddMemoryRegion(VoidPtr start, ULong sizeInBytes) {
        auto chunkPtr = ChunkAllocator->Allocate();
        auto* poolChunk = new(chunkPtr) PoolChunk((T*)start, sizeInBytes);
        RtlAvlTreeInsert(&ChunksTree, &(poolChunk->ThisChunkNode));
        return 0;
    }
    
    AVL_TREE ChunksTree;
    PoolAllocator<PoolChunk>* ChunkAllocator;
    
    enum class PoolChunkFreeResult : ULong {
        Success          = 0,
        OutOfChunkBlock  = 1,
        InvalidPointer   = 2,
        DoubleFree       = 3,
        NullPointerGiven = 4
    };
    
    class PoolChunk {
    public:
        PoolChunk() = default;
        
        PoolChunk(const T* startAddress, const ULong sizeInBytes) {
            StartAddress     = startAddress;
            TotalCount       = sizeInBytes / sizeof(T);
            AllocatedCount   = 0;
            Head             = StartAddress;
            TotalSizeInBytes = sizeInBytes;
            ThisChunkNode    = {};
#ifdef DEBUG
            auto debugFill   = (BytePtr)startAddress;
            
            for (Long i = 0; i < sizeInBytes; ++i) {
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
        
        T* Allocate(const Bool zeroFill = false) {
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
        
        ULong Free(const T* block) {
            if (block == nullptr)
                return (ULong)PoolChunkFreeResult::NullPointerGiven;
            
            if ((ULong)block >= (ULong)StartAddress + TotalSizeInBytes || (ULong)block < (ULong)StartAddress) {
                return (ULong)PoolChunkFreeResult::OutOfChunkBlock;
            }
            // Check double free?
            if (((ULong)block - (ULong)StartAddress) % sizeof(T) != 0) {
                return (ULong)PoolChunkFreeResult::InvalidPointer;
            }
            
            *(T**)block = Head;
            Head = block;
            AllocatedCount--;
            return (ULong)PoolChunkFreeResult::Success;
        }
        
        T* StartAddress;
        T* Head;
        ULong TotalCount;
        ULong AllocatedCount;
        ULong TotalSizeInBytes;
    public:
        AVL_TREE_NODE ThisChunkNode;
    };
};

}

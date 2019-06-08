//
// Created by Stoorx on 08.06.2019.
//

#pragma once

#include <Types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _StackAllocator {
    VoidPtr StartAddress;
    VoidPtr CurrentPosition;
    Word    MaxSize;
} StackAllocator;

Void StackAllocator$StackAllocator(StackAllocator* self, VoidPtr startAddress, Word maxSize);

VoidPtr StackAllocator$Allocate(StackAllocator* self, Word size);

VoidPtr StackAllocator$AllocateAligned(StackAllocator* self, Word size, Word alignment);

VoidPtr StackAllocator$Deallocate(StackAllocator* self, VoidPtr targetPtr);

VoidPtr StackAllocator$SetPosition(StackAllocator* self, VoidPtr targetPosition);

Void StackAllocator$Flush(StackAllocator* self);

Word StackAllocator$BytesAllocated(StackAllocator* self);

#ifdef __cplusplus
}
#endif
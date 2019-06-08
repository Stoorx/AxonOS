//
// Created by Stoorx on 08.06.2019.
//

#include <Memory/Allocators/StackAllocator.h>
#include <Null.h>

Void StackAllocator$StackAllocator(StackAllocator* self, VoidPtr startAddress, Word maxSize) {
    self->StartAddress    = startAddress;
    self->CurrentPosition = startAddress;
    self->MaxSize         = maxSize;
}

VoidPtr StackAllocator$Allocate(StackAllocator* self, Word size) {
    VoidPtr oldPosition = self->CurrentPosition;
    VoidPtr newPosition = self->CurrentPosition + size;
    
    if (newPosition >= self->StartAddress + self->MaxSize) {
        return NULL;
    }
    else {
        StackAllocator$SetPosition(self, newPosition);
        return oldPosition;
    }
}

VoidPtr StackAllocator$Deallocate(StackAllocator* self, VoidPtr targetPtr) {
    if (targetPtr > self->CurrentPosition || targetPtr < self->StartAddress) {
        return NULL;
    }
    else {
        self->CurrentPosition = targetPtr;
        return targetPtr;
    }
}

Void StackAllocator$Flush(StackAllocator* self) {
    StackAllocator$SetPosition(self, self->StartAddress);
}

VoidPtr StackAllocator$AllocateAligned(StackAllocator* self, Word size, Word alignment) {
    VoidPtr targetAddress = (VoidPtr)(((Word)self->CurrentPosition + alignment - 1) & ~(alignment - 1));
    
    VoidPtr paddingAllocation = StackAllocator$Allocate(self, targetAddress - self->CurrentPosition);
    if (paddingAllocation == NULL)
        return NULL;
    
    return StackAllocator$Allocate(self, size);
}

VoidPtr StackAllocator$SetPosition(StackAllocator* self, VoidPtr targetPosition) {
    if (targetPosition < self->StartAddress || targetPosition >= self->StartAddress + self->MaxSize) {
        return NULL;
    }
    else {
        self->CurrentPosition = targetPosition;
        return targetPosition;
    }
}

Word StackAllocator$BytesAllocated(StackAllocator* self) {
    return (Word)(self->CurrentPosition - self->StartAddress);
}

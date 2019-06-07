//
// Created by Stoorx on 07.06.2019.
//

#pragma once

#include <Types.h>

namespace Memory::Allocators {

class BuddyAllocator {
public:
    // Forward declarations
    class BuddyNode;
    
    class BuddyStack;
    
    // Constructors
    BuddyAllocator() = default;
    
    explicit BuddyAllocator(ULong granularity) : BuddyAllocator() {
        Granularity = granularity;
    }
    
    BuddyAllocator(BuddyStack* buddyStackList, ULong size, ULong granularity) {
        NodesStackList = buddyStackList;
        BuddiesCount   = size;
        Granularity    = granularity;
    }
    
    // Fields
    BuddyStack* NodesStackList;
    ULong BuddiesCount;
    ULong Granularity;
    
    // Methods
    VoidPtr Allocate(ULong size) {
        auto allocationSize = nextPower2(size);
        auto bucketNumber   = log2(allocationSize / Granularity);
        auto bucket = NodesStackList[bucketNumber];
        
        // TODO!!!
    }

private:
    
    // Finds the next power of 2 or, if v is a power of 2, return that.
    // From http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    ULong nextPower2(ULong v) {
        // We're not allocating chunks smaller than 2^BUDDY_BLOCK_ORDER bytes
        if (v < Granularity) {
            return Granularity;
        }
        
        // If v is a power of 2
        if (v && !(v & (v - 1))) {
            return v;
        }
        
        v--;
        v |= v >> 1u;
        v |= v >> 2u;
        v |= v >> 4u;
        v |= v >> 8u;
        v |= v >> 16u;
        v |= v >> 32u;
        v++;
        
        return v;
    }
    
    ULong log2(ULong v) {
        const ULong b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000, 0xFFFFFFFF00000000};
        const ULong S[] = {1, 2, 4, 8, 16, 32};
        int         i;
        
        ULong r = 0;
        for (i = 4; i >= 0; i--) {
            if (v & b[i]) {
                v >>= S[i];
                r |= S[i];
            }
        }
        return r;
    }

public:
    // Inner classes
    class BuddyStack {
    public:
        // Constructors
        BuddyStack() {
            Head       = nullptr;
            NodesCount = 0;
        }
        
        // Fields
        BuddyNode* Head;
        ULong NodesCount;
        
        // Methods
        void Push(BuddyNode* node) {
            node->NextNode = Head;
            Head = node;
            NodesCount++;
        }
        
        BuddyNode* Pop() {
            if (Head == nullptr)
                return nullptr;
            
            auto node = Head;
            Head = Head->NextNode;
            NodesCount--;
            return node;
        }
    };
    
    class BuddyNode {
    public:
        // Constructors
        BuddyNode(BuddyNode* nextNode, VoidPtr address, ULong nodeSize, Bool isAllocated) {
            NextNode    = nextNode;
            NodeSize    = nodeSize;
            IsAllocated = isAllocated;
            Address     = address;
        }
        
        // Fields
        BuddyNode* NextNode;
        VoidPtr Address;
        ULong   NodeSize;
        Bool    IsAllocated;
        // Methods
    };
};

}

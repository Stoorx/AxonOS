//
// Created by Stoorx on 26.10.2019.
//
#include <Types.h>

VoidPtr memset(VoidPtr dst, Int c, SizeT count) {
    BytePtr currentAddress   = dst;
    SizeT   currentCountLeft = count;
    UInt    fillValue        = (UInt)c & 0xFFu;
    fillValue = fillValue |
                fillValue << 8u |
                fillValue << 16u |
                fillValue << 24u;
    
    if (currentCountLeft == 0)
        return dst;
    
    if (((Word)currentAddress & 0x1u) != 0) { // align to even address
        *currentAddress++ = (Byte)fillValue;
        currentCountLeft--;
    }
    
    if (currentCountLeft == 0)
        return dst;
    
    if (currentCountLeft == 1) { // if one element left
        *currentAddress = (Byte)fillValue;
        return dst;
    }
    
    if (((Word)currentAddress & 0x2u) != 0) { // align to 4th address
        *((UShortPtr)currentAddress++) = (UShort)fillValue;
        currentCountLeft -= sizeof(UShort);
    }
    
    if (currentCountLeft == 0)
        return dst;
    
    // fill with ints while it is possible
    for (; currentCountLeft >= sizeof(UInt); currentCountLeft -= sizeof(UInt)) {
        *((UIntPtr)currentAddress++) = fillValue;
    }
    
    if (currentCountLeft == 0)
        return dst;
    
    if (currentCountLeft == 1) { // if one element left
        *currentAddress = (Byte)fillValue;
        return dst;
    }
    
    // 2 or 3 elements left
    *((UShortPtr)currentAddress++) = (UShort)fillValue;
    currentCountLeft -= sizeof(UShort);
    
    if (currentCountLeft == 0)
        return dst;
    
    // if one element left
    *currentAddress = (Byte)fillValue;
    return dst;
}

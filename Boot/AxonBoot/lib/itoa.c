//
// Created by Stoorx on 02.11.2019.
//

#include "itoa.h"
#include "memset.h"

Char8Ptr itoa(Int number, Char8Ptr buffer, Byte radix) {
    Char8 buf[32];
    memset(buf, 0, 32);
    
    Int div = number;
    
    int inIndex = 0;
    for (; div != 0; div /= radix) {
        buf[inIndex++] = div % radix;
    }
    
    Int outIndex = 0;
    if (number < 0)
        buffer[outIndex++] = '-';
    
    for (; inIndex != 0; inIndex--) {
        buffer[outIndex++] = buf[inIndex - 1] + '0';
    }
    buffer[outIndex]       = '\0';
    
    return buffer;
}

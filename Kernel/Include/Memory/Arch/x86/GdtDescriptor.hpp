//
// Created by Stoorx on 02.06.2019.
//

#pragma once

#include <Types.h>

#pragma pack(push, 1)

struct GdtDescriptor {
    UShort LimitLow;
    UShort BaseLow;
    
    Byte BaseMid;
    
    struct {
        unsigned Accessed : 1;
        unsigned ReadableWriteable : 1;
        unsigned DirectionConforming : 1;
        unsigned Executable : 1;
        unsigned System : 1;
        unsigned DPL : 2;
        unsigned Present : 1;
    }    AccessByte;
    
    union {
        Byte LimitHigh;
        struct {
        private:
            unsigned __padding : 4;
        public:
            unsigned Avail : 1;
            unsigned Long : 1;
            unsigned Size : 1;
            unsigned Granularity : 1;
        }    Flags;
    };
    
    Byte BaseHigh;
};
#pragma pack(pop)

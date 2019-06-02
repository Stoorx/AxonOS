//
// Created by Stoorx on 02.06.2019.
//

#pragma once

#include <Types.h>

#pragma pack(push, 1)

union GdtDescriptor {
    GdtDescriptor(UInt base, UInt limit, Byte accessByte, Byte flags) : AccessByte(), Flags() {
        LimitLow = (UShort)(limit & 0xFFFF);
        BaseLow  = (UShort)(base & 0xFFFF);
        BaseMid  = (Byte)((base & 0xFF'0000) >> 16);
        
        BaseHigh = (Byte)((base & 0xFF00'0000) >> 24);
        
        AccessByte.RawValue = accessByte;
        Flags.RawValue      = (Byte)((flags & 0xF) << 4);
        Flags.LimitHigh     = (Byte)((limit & 0xF'0000) >> 16);
    }
    
    ULong RawValue;
    struct {
        UShort LimitLow;
        UShort BaseLow;
        
        Byte BaseMid;
        
        union {
            Byte RawValue;
            struct {
                unsigned Accessed : 1;
                unsigned ReadableWriteable : 1;
                unsigned DirectionConforming : 1;
                unsigned Executable : 1;
                unsigned System : 1;
                unsigned DPL : 2;
                unsigned Present : 1;
            };
        }    AccessByte;
        
        union {
            Byte RawValue;
            struct {
                unsigned LimitHigh : 4;
                unsigned Avail : 1;
                unsigned Long : 1;
                unsigned Size : 1;
                unsigned Granularity : 1;
            };
        }    Flags;
        
        Byte BaseHigh;
    };
};

#pragma pack(pop)

enum class SystemDescriptorType : Byte {
    Free16BitTss       = 0b0001,
    Ldt                = 0b0010,
    Busy16BitTss       = 0b0011,
    CallGate16Bit      = 0b0100,
    TaskGate           = 0b0101,
    InterruptGate16Bit = 0b0110,
    TrapGate16Bit      = 0b0111,
    Free32BitTss       = 0b1001,
    Busy32BitTss       = 0b1011,
    CallGate32Bit      = 0b1100,
    InterruptGate32Bit = 0b1110,
    TrapGate32Bit      = 0b1111
};

enum class UserSegmentDescriptorType : Byte {
    DataReadOnly          = 0b0000,
    DataReadWrite         = 0b0010,
    DataReadOnlyGrowDown  = 0b0100,
    DataReadWriteGrowDown = 0b0110,
    
    CodeExecuteOnly           = 0b1000,
    CodeExecuteRead           = 0b1010,
    CodeExecuteOnlyConforming = 0b1100,
    CodeExecuteReadConforming = 0b1110
};

enum class AccessByteValues : Byte {
    Accessed            = 1,
    ReadableWriteable   = 2,
    DirectionConforming = 4,
    Executable          = 8,
    UserSegment         = 16,
    DPL0                = 0,
    DPL1                = 32,
    DPL2                = 64,
    DPL3                = 64 + 32,
    Present             = 128
};

enum class FlagsValues : Byte {
    Avail       = 1,
    Long        = 2,
    Size        = 4,
    Granularity = 8
};
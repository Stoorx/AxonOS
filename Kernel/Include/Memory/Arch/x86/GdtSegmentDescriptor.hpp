//
// Created by Stoorx on 02.06.2019.
//

#pragma once

#include <Types.h>

#pragma pack(push, 1)

enum class AccessByteFlag : Byte;
enum class SegmentFlag : Byte;

class GdtSegmentDescriptor {
public:
    
    GdtSegmentDescriptor() = default;
    
    explicit GdtSegmentDescriptor(const ULong rawValue) {
        RawValue = rawValue;
    }
    
    GdtSegmentDescriptor& SetBase(const UInt base) {
        auto  baseLow  = (UShort)base;
        auto  baseMid  = (Byte)(base >> 16u);
        auto  baseHigh = (Byte)(base >> 24u);
        ULong mask     = 0xFF00'00FF'FFFF'0000u;
        
        RawValue = (RawValue & ~mask) |
                   ((ULong)baseLow << 16u) |
                   ((ULong)baseMid << 32u) |
                   ((ULong)baseHigh << 56u);
        return *this;
    }
    
    UInt GetBase() {
        ULong mask   = 0xFF00'00FF'FFFF'0000u;
        ULong masked = RawValue & mask;
        UInt  base   = ((masked >> 16u) & 0xFF'FFFFu) | ((masked >> 56u) << 24u);
        return base;
    }
    
    GdtSegmentDescriptor& SetLimit(const UInt limit) {
        auto  limitLow  = (UShort)limit;
        auto  limitHigh = (Byte)((limit >> 16u) & 0xFu);
        ULong mask      = 0x000F'0000'0000'FFFF;
        
        RawValue = (RawValue & ~mask) |
                   ((ULong)limitLow) |
                   ((ULong)limitHigh << 48u);
        return *this;
    }
    
    UInt GetLimit() {
        ULong mask   = 0x000F'0000'0000'FFFF;
        ULong masked = RawValue & mask;
        UInt  limit  = (masked & 0xFFFFu) |
                       (((masked >> 48u) & 0xFu) << 16u);
        return limit;
    }
    
    GdtSegmentDescriptor& SetAccessByte(const Byte accessByte) {
        ULong mask = 0x0000'FF00'0000'0000u;
        RawValue = (RawValue & ~mask) |
                   ((ULong)accessByte << 40u);
        return *this;
    }
    
    Byte GetAccessByte() {
        return RawValue >> 40u;
    }
    
    GdtSegmentDescriptor& SetAccessByteFlag(const AccessByteFlag flag, const Bool value = true) {
        if (value) {
            RawValue |= ((ULong)flag << 40u);
        }
        else {
            RawValue &= ~((ULong)flag << 40u);
        }
        return *this;
    }
    
    Bool GetAccessByteFlag(const AccessByteFlag flag) {
        return ((RawValue >> 40u) & (Byte)flag) != 0;
    }
    
    GdtSegmentDescriptor& SetSegmentFlag(const SegmentFlag flag, const Bool value = true) {
        if (value) {
            RawValue |= ((ULong)flag << 52u);
        }
        else {
            RawValue &= ~((ULong)flag << 52u);
        }
        return *this;
    }
    
    Bool GetSegmentFlag(const SegmentFlag flag) {
        return ((RawValue >> 52u) & (Byte)flag) != 0;
    }
    
    GdtSegmentDescriptor& SetSegmentFlags(const Byte flags) {
        ULong mask = 0x00F0'0000'0000'0000;
        RawValue = (RawValue & ~mask) |
                   ((ULong)(flags & 0xFu) << 52u);
        return *this;
    }
    
    Byte GetSegmentFlags() {
        ULong mask = 0x00F0'0000'0000'0000;
        return (RawValue & mask) >> 52u;
    }
    
    ULong RawValue = 0;
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

enum class SegmentDescriptorType : Byte {
    DataReadOnly          = 0b10000,
    DataReadWrite         = 0b10010,
    DataReadOnlyGrowDown  = 0b10100,
    DataReadWriteGrowDown = 0b10110,
    
    CodeExecuteOnly           = 0b11000,
    CodeExecuteRead           = 0b11010,
    CodeExecuteOnlyConforming = 0b11100,
    CodeExecuteReadConforming = 0b11110
};

enum class AccessByteFlag : Byte {
    Accessed      = 1,
    ReadableCode  = 2,
    WriteableData = 2,
    Direction     = 4,
    Conforming    = 4,
    Executable    = 8,
    UserSegment   = 16,
    DPL0          = 0,
    DPL1          = 32,
    DPL2          = 64,
    DPL3          = 96,
    Present       = 128
};

enum class SegmentFlag : Byte {
    Avail       = 1,
    Long        = 2,
    Size        = 4,
    Granularity = 8
};
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
    
    explicit GdtSegmentDescriptor(ULong rawValue);
    
    GdtSegmentDescriptor& SetBase(UInt base);
    
    UInt GetBase();
    
    GdtSegmentDescriptor& SetLimit(UInt limit);
    
    UInt GetLimit();
    
    GdtSegmentDescriptor& SetAccessByte(Byte accessByte);
    
    Byte GetAccessByte();
    
    GdtSegmentDescriptor& SetAccessByteFlag(AccessByteFlag flag, Bool value = true);
    
    Bool GetAccessByteFlag(AccessByteFlag flag);
    
    GdtSegmentDescriptor& SetSegmentFlag(SegmentFlag flag, Bool value = true);
    
    Bool GetSegmentFlag(SegmentFlag flag);
    
    GdtSegmentDescriptor& SetSegmentFlags(Byte flags);
    
    Byte GetSegmentFlags();
    
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
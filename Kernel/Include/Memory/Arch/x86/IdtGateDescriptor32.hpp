//
// Created by Stoorx on 04.06.2019.
//

#pragma once


enum class GateAttributesFlag : Byte;

#pragma pack(push, 1)

class IdtGateDescriptor32 {
public:
    
    IdtGateDescriptor32() = default;
    
    IdtGateDescriptor32& SetOffset(UInt offset);
    
    UInt GetOffset();
    
    IdtGateDescriptor32& SetSelector(UShort selector);
    
    UShort GetSelector();
    
    IdtGateDescriptor32& SetTypeAttributesByte(Byte typeAttributesByte);
    
    Byte GetTypeAttributesByte();
    
    IdtGateDescriptor32& SetTypeAttributeFlag(GateAttributesFlag flag, Bool value = true);
    
    Bool GetTypeAttributeFlag(GateAttributesFlag flag);

private:
    UShort offsetLow;
    UShort segmentSelector;
    Byte   Reserved;
    Byte   typeAttributes;
    UShort offsetHigh;
};

#pragma pack(pop)

enum class GateAttributesFlag : Byte {
    TaskGate32      = 0b0101,
    InterruptGate16 = 0b0110,
    TrapGate16      = 0b0111,
    InterruptGate32 = 0b1110,
    TrapGate32      = 0b1111,
    DPL0            = 0,
    DPL1            = 32,
    DPL2            = 64,
    DPL3            = 96,
    Present         = 128
};
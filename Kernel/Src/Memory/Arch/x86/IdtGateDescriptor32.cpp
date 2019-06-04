//
// Created by Stoorx on 04.06.2019.
//

#include <Types.h>
#include <Memory/Arch/x86/IdtGateDescriptor32.hpp>

IdtGateDescriptor32& IdtGateDescriptor32::SetOffset(const UInt offset) {
    offsetLow  = (UShort)offset;
    offsetHigh = (UShort)(offset >> 16u);
    return *this;
}

UInt IdtGateDescriptor32::GetOffset() {
    return ((UInt)offsetHigh << 16u) | ((UInt)offsetLow);
}

IdtGateDescriptor32& IdtGateDescriptor32::SetSelector(const UShort selector) {
    segmentSelector = selector;
    return *this;
}

UShort IdtGateDescriptor32::GetSelector() {
    return segmentSelector;
}

IdtGateDescriptor32& IdtGateDescriptor32::SetTypeAttributesByte(const Byte typeAttributesByte) {
    typeAttributes = typeAttributesByte;
    return *this;
}

Byte IdtGateDescriptor32::GetTypeAttributesByte() {
    return typeAttributes;
}

IdtGateDescriptor32& IdtGateDescriptor32::SetTypeAttributeFlag(const GateAttributesFlag flag, const Bool value) {
    if (value) {
        typeAttributes |= (Byte)flag;
    }
    else {
        typeAttributes &= (Byte)(~(Byte)flag); // Potentially dangerous!
    }
    return *this;
}

Bool IdtGateDescriptor32::GetTypeAttributeFlag(GateAttributesFlag flag) {
    return (typeAttributes & (Byte)flag) != 0;
}

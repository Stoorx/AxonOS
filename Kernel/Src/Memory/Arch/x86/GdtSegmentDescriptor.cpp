//
// Created by Stoorx on 04.06.2019.
//

#include <Memory/Arch/x86/GdtSegmentDescriptor.hpp>
#include <Types.h>

GdtSegmentDescriptor& GdtSegmentDescriptor::SetBase(const UInt base) {
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

Byte GdtSegmentDescriptor::GetSegmentFlags() {
    ULong mask = 0x00F0'0000'0000'0000;
    return (RawValue & mask) >> 52u;
}

GdtSegmentDescriptor& GdtSegmentDescriptor::SetSegmentFlags(const Byte flags) {
    ULong mask = 0x00F0'0000'0000'0000;
    RawValue = (RawValue & ~mask) |
               ((ULong)(flags & 0xFu) << 52u);
    return *this;
}

Bool GdtSegmentDescriptor::GetSegmentFlag(const SegmentFlag flag) {
    return ((RawValue >> 52u) & (Byte)flag) != 0;
}

GdtSegmentDescriptor& GdtSegmentDescriptor::SetSegmentFlag(const SegmentFlag flag, const Bool value) {
    if (value) {
        RawValue |= ((ULong)flag << 52u);
    }
    else {
        RawValue &= ~((ULong)flag << 52u);
    }
    return *this;
}

Bool GdtSegmentDescriptor::GetAccessByteFlag(const AccessByteFlag flag) {
    return ((RawValue >> 40u) & (Byte)flag) != 0;
}

GdtSegmentDescriptor& GdtSegmentDescriptor::SetAccessByteFlag(const AccessByteFlag flag, const Bool value) {
    if (value) {
        RawValue |= ((ULong)flag << 40u);
    }
    else {
        RawValue &= ~((ULong)flag << 40u);
    }
    return *this;
}

GdtSegmentDescriptor& GdtSegmentDescriptor::SetAccessByte(const Byte accessByte) {
    ULong mask = 0x0000'FF00'0000'0000u;
    RawValue = (RawValue & ~mask) |
               ((ULong)accessByte << 40u);
    return *this;
}

Byte GdtSegmentDescriptor::GetAccessByte() {
    return RawValue >> 40u;
}

UInt GdtSegmentDescriptor::GetLimit() {
    ULong mask   = 0x000F'0000'0000'FFFF;
    ULong masked = RawValue & mask;
    UInt  limit  = (masked & 0xFFFFu) |
                   (((masked >> 48u) & 0xFu) << 16u);
    return limit;
}

GdtSegmentDescriptor& GdtSegmentDescriptor::SetLimit(const UInt limit) {
    auto  limitLow  = (UShort)limit;
    auto  limitHigh = (Byte)((limit >> 16u) & 0xFu);
    ULong mask      = 0x000F'0000'0000'FFFF;
    
    RawValue = (RawValue & ~mask) |
               ((ULong)limitLow) |
               ((ULong)limitHigh << 48u);
    return *this;
}

UInt GdtSegmentDescriptor::GetBase() {
    ULong mask   = 0xFF00'00FF'FFFF'0000u;
    ULong masked = RawValue & mask;
    UInt  base   = ((masked >> 16u) & 0xFF'FFFFu) | ((masked >> 56u) << 24u);
    return base;
}

GdtSegmentDescriptor::GdtSegmentDescriptor(const ULong rawValue) {
    RawValue = rawValue;
}

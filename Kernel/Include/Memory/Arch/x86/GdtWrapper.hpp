//
// Created by Stoorx on 02.06.2019.
//

#pragma once

#include "GdtSegmentDescriptor.hpp"

class GdtWrapper {
protected:
    GdtSegmentDescriptor* Table;
    UShort Count;
};

#pragma pack(push, 1)

struct GdtLoadDescriptor {
    GdtLoadDescriptor(UShort count, GdtSegmentDescriptor* address) :
        Size(sizeof(GdtSegmentDescriptor) * count - 1),
        Address(address){}
    
    UShort Size;
    GdtSegmentDescriptor* Address;
};

#pragma pack(pop)

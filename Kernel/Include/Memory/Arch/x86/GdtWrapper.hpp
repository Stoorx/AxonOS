//
// Created by Stoorx on 02.06.2019.
//

#pragma once

#include "GdtDescriptor.hpp"

class GdtWrapper {
protected:
    GdtDescriptor* Table;
    UShort Count;
};

#pragma pack(push, 1)

struct GdtLoadDescriptor32 {
    GdtLoadDescriptor32(UShort count, GdtDescriptor* address) :
        Size(sizeof(GdtDescriptor) * count - 1),
        Address(address)
    
    UShort Size;
    GdtDescriptor* Address;
};

#pragma pack(pop)

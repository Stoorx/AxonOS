//
// Created by Stoorx on 04.06.2019.
//

#include <Types.h>
#include <Memory/Arch/x86/IdtGateDescriptor32.hpp>
#include <Catch.hpp>

TEST_CASE("IdtGateDescriptor32 size") {
    REQUIRE(sizeof(IdtGateDescriptor32) == 8);
}

TEST_CASE("IdtGateDescriptor32 construct") {
    auto desc = IdtGateDescriptor32()
        .SetOffset(0x15)
        .SetSelector(0x8)
        .SetTypeAttributesByte((Byte)GateAttributesFlag::TrapGate32)
        .SetTypeAttributeFlag(GateAttributesFlag::Present);
    
    REQUIRE(desc.GetOffset() == 0x15);
    REQUIRE(desc.GetSelector() == 0x8);
    REQUIRE(desc.GetTypeAttributesByte() == 0x8F);
    
}

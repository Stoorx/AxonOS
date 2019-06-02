//
// Created by Stoorx on 02.06.2019.
//

#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <Memory/Arch/x86/GdtDescriptor.hpp>

TEST_CASE("GDT Descriptor length") {
    REQUIRE(sizeof(GdtDescriptor) == 8);
}

TEST_CASE("GDT Descriptor constructor") {
    auto desc = GdtDescriptor(
        0,
        0xFFFF'FFFF,
        (Byte)UserSegmentDescriptorType::CodeExecuteRead | (Byte)AccessByteValues::DPL0 |
        (Byte)AccessByteValues::Present | (Byte)AccessByteValues::UserSegment,
        (Byte)FlagsValues::Granularity | (Byte)FlagsValues::Long
    );
    REQUIRE(desc.RawValue == 0x00AF'9A00'0000'FFFF);
}


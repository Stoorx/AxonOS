//
// Created by Stoorx on 02.06.2019.
//

#include <Catch.hpp>
#include <Memory/Arch/x86/GdtSegmentDescriptor.hpp>

TEST_CASE("GDT Descriptor length") {
    REQUIRE(sizeof(GdtSegmentDescriptor) == 8);
}

TEST_CASE("GDT Descriptor constructor") {
    GdtSegmentDescriptor desc = GdtSegmentDescriptor()
        .SetBase(0)
        .SetLimit(0xF'FFFFu)
        .SetAccessByte((Byte)SegmentDescriptorType::CodeExecuteRead)
        .SetAccessByteFlag(AccessByteFlag::DPL0)
        .SetAccessByteFlag(AccessByteFlag::Present)
        .SetSegmentFlag(SegmentFlag::Granularity)
        .SetSegmentFlag(SegmentFlag::Long);
    
    // Test construct
    REQUIRE(desc.RawValue == 0x00AF'9A00'0000'FFFF);
    
    // Test getters
    REQUIRE(desc.GetBase() == 0);
    REQUIRE(desc.GetLimit() == 0xF'FFFF);
    REQUIRE(desc.GetSegmentFlag(SegmentFlag::Long));
    REQUIRE(desc.GetAccessByteFlag(AccessByteFlag::Present));
    
    
    // Null descriptor test
    desc = GdtSegmentDescriptor();
    REQUIRE(desc.RawValue == 0x0);
}

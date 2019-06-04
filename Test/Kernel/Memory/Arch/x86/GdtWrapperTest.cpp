//
// Created by Stoorx on 03.06.2019.
//


#include <Catch.hpp>
#include <Memory/Arch/x86/GdtWrapper.hpp>
#include <Types.h>

TEST_CASE("GdtLoadDescriptor size") {
    switch (sizeof(VoidPtr)) {
        case 4:
            REQUIRE(sizeof(GdtLoadDescriptor) == 6);
            break;
        case 8:
            REQUIRE(sizeof(GdtLoadDescriptor) == 10);
            break;
        default:
            CATCH_ERROR("Strange pointer size");
    }
    
}
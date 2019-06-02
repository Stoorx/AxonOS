//
// Created by Stoorx on 02.06.2019.
//

#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <Memory/Arch/x86/GdtDescriptor.hpp>

TEST_CASE("GDT Descriptor length") {
    REQUIRE(sizeof(GdtDescriptor) == 8);
}


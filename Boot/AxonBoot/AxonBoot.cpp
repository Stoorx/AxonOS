#include "AxonBoot.hpp"

void AxonBoot::main() {
    *((int*)0xB8000) = 0x07320733; // Prints "32"
}

#include "AxonBoot.hpp"
#include <version.h>


void print(const char* message) {
    auto* frameBuffer = (unsigned short*)0xB8000u;
    
    for (auto* currentChar = (unsigned char*)message;
         *currentChar != '\0';
         currentChar++, frameBuffer++) {
        *frameBuffer = *currentChar | 0x0700u;
    }
}


void AxonBoot::main() {
    const char* ver = KERNEL_VERSION_STR;
    print(ver);
}

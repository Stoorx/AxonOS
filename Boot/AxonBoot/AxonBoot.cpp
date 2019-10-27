#include "AxonBoot.hpp"
#include <version.h>
#include <Util/EarlyScreenPrinter.hpp>


void AxonBoot::main() {
    auto printer = EarlyScreenPrinter();
    printer.print("AxonBoot-");
    
    printer.println("1234");
    
    printer.println(KERNEL_VERSION_STR);
    
    int chars = 0x30303030;
    while (true) {
        printer.println((Char8Ptr)&chars);
        chars++;
    }
    
}

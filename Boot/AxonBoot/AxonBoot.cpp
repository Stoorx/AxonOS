#include "AxonBoot.hpp"
#include <version.h>
#include <Util/EarlyScreenPrinter.hpp>
#include <x86/RealModeCall.h>
#include <itoa.h>

void AxonBoot::main() {
    RealModeContext rmContext = {};
    realModeIntExecute(&rmContext, 0x12);
    Char8 bf[64];
    itoa(rmContext.eax, bf, 10);
    
    auto printer = EarlyScreenPrinter();
    printer.println(bf);
    
}

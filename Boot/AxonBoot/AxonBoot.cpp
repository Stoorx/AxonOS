#include "AxonBoot.hpp"
#include <version.h>
#include <Util/EarlyScreenPrinter.hpp>
#include <x86/RealModeCall.h>

void AxonBoot::main() {
    
    RealModeContext rmContext = {};
    rmContext.eax = 0x0003;
    realModeIntExecute(&rmContext, 0x10);
    
    auto printer = EarlyScreenPrinter();
    printer.print("AxonBoot-");
    printer.println(KERNEL_VERSION_STR);
}

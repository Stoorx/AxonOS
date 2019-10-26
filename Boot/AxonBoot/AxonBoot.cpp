#include "AxonBoot.hpp"
#include <version.h>
#include <Util/EarlyScreenPrinter.hpp>



void AxonBoot::main() {
    auto printer = EarlyScreenPrinter();
    printer.println("AxonBoot");
    printer.println(KERNEL_VERSION_STR);
}

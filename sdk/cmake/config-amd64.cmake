
set(SubArch "" CACHE STRING
        "Sub-architecture of system.")

set(TuneArch "athlon64" CACHE STRING
        "Tune instructions for CPU type. One of: k8 opteron athlon64 athlon-fx")

set(OptimizeLevel "4" CACHE STRING
        "What level of optimization to use.
            0 = off
            1 = Default option, optimize for size (-Os) with some additional options
            2 = Optimize for size (-Os)
            3 = Optimize debugging experience (-Og)
            4 = Optimize (-O1)
            5 = Optimize for release (-O2)
            6 = Optimize extremely (-O3)
            7 = Disregard strict standards compliance (-Ofast)"
        )

set(KernelDebugger FALSE CACHE BOOL
        "Use kernel debugger.")

set(BuildUser TRUE CACHE BOOL
        "Build user mode applications.")

add_definitions(-D_AMD64_ -D_X86_64_)
add_definitions(-U_X86_ -U_i386_)

set(AltArchName "x86_64")

set(src MBR.asm)

set(ASM_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/${src})
set(ASM_OUT ${CMAKE_CURRENT_BINARY_DIR}/${src}.bin)
add_custom_command(
        OUTPUT ${ASM_OUT}
        COMMAND ${NASM_EXECUTABLE}
        ARGS -o ${ASM_OUT} -f bin -- ${ASM_SOURCE}
        DEPENDS ${ASM_SOURCE}
)
add_custom_target(x86_MBR ${ASM_OUT} DEPENDS ${ASM_OUT} USES_TERMINAL) # FIXME!!
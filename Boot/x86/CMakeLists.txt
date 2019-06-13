set(src MBR.asm)

set(ASM_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
set(ASM_OUT "${CMAKE_CURRENT_BINARY_DIR}/${src}.bin")
add_custom_command(
        OUTPUT ${ASM_OUT}
        COMMAND ${NASM_EXECUTABLE}
        ARGS -o $<SHELL_PATH:${ASM_OUT}> -f bin $<SHELL_PATH:${ASM_SOURCE}>
        DEPENDS ${ASM_SOURCE}
)
add_custom_target(x86_MBR ${ASM_OUT} DEPENDS ${ASM_OUT})

add_custom_command(
        OUTPUT ${DIST_DIR}/Boot/MBR.bin
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${ASM_OUT} ${DIST_DIR}/Boot/MBR.bin
        DEPENDS ${ASM_OUT}
)

add_custom_target(x86_MBR_dist ${DIST_DIR}/Boot/MBR.bin DEPENDS ${DIST_DIR}/Boot/MBR.bin)

set(mbr_src MBR)
set(vbr_src VBR_FAT32)

set(MBR_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${mbr_src}.asm")
set(MBR_OUT "${CMAKE_CURRENT_BINARY_DIR}/${mbr_src}.bin")
add_custom_command(
        OUTPUT ${MBR_OUT}
        COMMAND ${NASM_EXECUTABLE}
        ARGS -o $<SHELL_PATH:${MBR_OUT}> -f bin $<SHELL_PATH:${MBR_SOURCE}>
        DEPENDS ${MBR_SOURCE}
)
add_custom_target(x86_MBR ${MBR_OUT} DEPENDS ${MBR_OUT})

add_custom_command(
        OUTPUT ${DIST_DIR}/temp/MBR.bin
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${MBR_OUT} ${DIST_DIR}/temp/MBR.bin
        DEPENDS ${MBR_OUT}
)

add_custom_target(x86_MBR_dist ALL DEPENDS ${DIST_DIR}/temp/MBR.bin)


set(VBR_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${vbr_src}.asm")
set(VBR_OUT "${CMAKE_CURRENT_BINARY_DIR}/${vbr_src}.bin")
add_custom_command(
        OUTPUT ${VBR_OUT}
        COMMAND ${NASM_EXECUTABLE}
        ARGS -o $<SHELL_PATH:${VBR_OUT}> -f bin $<SHELL_PATH:${VBR_SOURCE}>
        DEPENDS ${VBR_SOURCE}
)
add_custom_target(x86_VBR ${VBR_OUT} DEPENDS ${VBR_OUT})

add_custom_command(
        OUTPUT ${DIST_DIR}/temp/VBR.bin
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${VBR_OUT} ${DIST_DIR}/temp/VBR.bin
        DEPENDS ${VBR_OUT}
)

add_custom_target(x86_VBR_dist ALL DEPENDS ${DIST_DIR}/temp/VBR.bin)

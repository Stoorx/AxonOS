set(startup_src Startup)

set(STARTUP_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${startup_src}.asm")
set(STARTUP_OUT "${CMAKE_CURRENT_BINARY_DIR}/${startup_src}.bin")
add_custom_command(
        OUTPUT ${STARTUP_OUT}
        COMMAND ${NASM_EXECUTABLE}
        ARGS -o $<SHELL_PATH:${STARTUP_OUT}> -f bin $<SHELL_PATH:${STARTUP_SOURCE}>
        DEPENDS ${STARTUP_SOURCE}
)
add_custom_target(x86_Startup ${STARTUP_OUT} DEPENDS ${STARTUP_OUT})

add_custom_command(
        OUTPUT ${DIST_DIR}/temp/AXONBOOT
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${STARTUP_OUT} ${DIST_DIR}/temp/AXONBOOT
        DEPENDS ${STARTUP_OUT}
)

add_custom_target(x86_Startup_dist ALL DEPENDS ${DIST_DIR}/temp/AXONBOOT)

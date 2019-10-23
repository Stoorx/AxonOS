function(configure_host_tools hostToolsDir)
    file(MAKE_DIRECTORY ${hostToolsDir})

    message(STATUS "Configuring host tools...")

    set_property(SOURCE host_tools PROPERTY SYMBOLIC 1)

    # Make a host-tools target so it'll be built when needed
    # custom target + symbolic output prevents cmake from running
    # the command multiple times per build
    add_custom_command(
            COMMAND ${CMAKE_COMMAND} --build ${hostToolsDir}
            OUTPUT host_tools)
    add_custom_target(build-host-tools ALL DEPENDS host_tools)

endfunction()

set(HostToolsDir ${CMAKE_BINARY_DIR}/host-tools)
configure_host_tools(${HostToolsDir})

# TODO: understand what is it and fix

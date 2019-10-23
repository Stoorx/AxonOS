OUTPUT_FORMAT("binary")

_AXONBOOT_START_ADDR = 0x1000;

SECTIONS {
    . = _AXONBOOT_START_ADDR;
    .text16 : {
        _TEXT16_START = .;
        */Startup.asm.o (.text16)
        *.o(.text16)
        _TEXT16_END = .;
    }
    .data16 : {
        _DATA16_START = .;
        *.o(.data16)
        _DATA16_END = .;
    }
    .text32 : ALIGN(0x1000) {
        _TEXT32_START = .;
        *.o(.text)
        _TEXT32_END = .;
    }
    .data32 : ALIGN(0x1000) {
        _DATA32_START = .;
        *.o(.data, .bss)
        _DATA32_END = .;
    }
    .rodata32 : ALIGN(0x1000) {
        _RODATA32_START = .;
        *.o(.rodata)
        _RODATA32_END = .;
    }
    .eh_frame : {
        *.o(.eh_frame)
    }
    _AXONBOOT_END_ADDR = .;
    /DISCARD/ : {
        *(.comment)
    }
}

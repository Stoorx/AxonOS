;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  AxonBoot bootloader                 ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Memory map                          ;;
;;                   physical address   ;;
;;  +------------------------+ 00000H   ;;
;;  | Interrupt Vector Table |          ;;
;;  +------------------------+ 00400H   ;;
;;  |     BIOS Data Area     |          ;;
;;  +------------------------+ 00500H   ;;
;;  | PrtScr Status / Unused |          ;;
;;  +------------------------+ 00600H   ;;
;;  |       VBR (512 B)      |          ;;
;;  +------------------------+ 00800H   ;;
;;  |      VBR II (512 B)    |          ;;
;;  +------------------------+ 00A00H   ;;
;;  |    VBR stack (512 B)   |          ;;
;;  +------------------------+ 00C00H   ;;
;;  |    Bootloader stack    |          ;;
;;  +------------------------+ 01000H   ;;
;;  |   BootLoader (508 KB)  |          ;;
;;  +------------------------+ 80000H   ;;
;;  |    Available Memory    |          ;;
;;  +------------------------+ 9FC00H   ;;
;;  |          EBDA          |          ;;
;;  +------------------------+ A0000H   ;;
;;  |    Video RAM & ROM     |          ;;
;;  +------------------------+ FFFFFH   ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%define BOOT_ADDR 0x1000
%define LOADER_STACK_16 0x1000
%define LOADER_STACK_32 0x80000

extern main

[SECTION .text16]
;[ORG BOOT_ADDR]
[BITS 16]
start:
    ; Context:
    ; DL = drive number
    ; CS=0x0, IP = 0x1000

    ; init registers
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, LOADER_STACK_16
    sti

    in al, 0x92
    test al, 2
    jnz .a20_enabled
    or al, 2
    and al, 0xFE
    out 0x92, al
    .a20_enabled:

    cli

    ; Set PE flag
    mov eax, cr0
    or al, 1
    mov cr0, eax

    push dword GDT
    push word (GDT.end - GDT - 1)
    mov bp, sp
    lgdt [bp]

    jmp 0x8:.fake_protected_entry

[BITS 32]
.fake_protected_entry:
    jmp protected_entry
[BITS 16]

[SECTION .data16]
align 8
GDT:
.null_desctiptor:
    dd 0x0
    dd 0x0
.code32_descriptor:
    dw 0xFFFF ; Limit
    dw 0x0 ; Base
    db 0x0 ; Base
    db 0x9A ; Type: Present, DPL0, Code, Readable
    db 0xCF ; Granularity in pages, 32 bit
    db 0x0 ; Base
.data32_descriptor:
    dw 0xFFFF ; Limit
    dw 0x0 ; Base
    db 0x0 ; Base
    db 0x92 ; Type: Present, DPL0, Data, Writeable
    db 0xCF ; Granularity in pages, 32 bit
    db 0x0 ; Base
.end:

[SECTION .text]
[BITS 32]
protected_entry:
    mov ax, 16 ; Data selector
    mov ds, ax
    mov ss, ax
    mov es, ax
    ;mov dword [0xB8000], 0x07320733 ; Prints "32"
    ; TODO: initialize protected stack and C-decl function call
    call main
    ; pass
hlt_system:
    cli
    hlt
    jmp short hlt_system
[SECTION .data]
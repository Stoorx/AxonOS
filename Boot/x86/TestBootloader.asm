;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Test bootloader                     ;;
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
%define LOADER_STACK 0x1000

[BITS 16]
[ORG BOOT_ADDR]
start:
    ; Context:
    ; DL = drive number
    ; CS=0x0, IP = 0x1000

    ; init registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, LOADER_STACK

    ; Clear screen
    mov ax, 0x3
    int 0x10

    mov si, test_message
    ; pass
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Printing function       ;;
;;  DS:SI - message         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
print_and_hlt:
    mov di, 0
    mov ax, 0xB800
    mov es, ax
    cld
    mov ah, 7
print_loop:
    lodsb
    test al, al
    jz short hlt_system
    stosw
    jmp short print_loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Halt system             ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
hlt_system:
    cli
    hlt
    jmp short hlt_system
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Data                    ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
test_message:
    db "Hello from this test bootloader! If you can read this text, the MBR and VBR had worked correctly.",0
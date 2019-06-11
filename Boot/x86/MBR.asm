;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Main Boot Record                    ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                      ;;
;;                                      ;;
;;  Memory map                          ;;
;;                   physical address   ;;
;;  +------------------------+ 00000H   ;;
;;  | Interrupt Vector Table |          ;;
;;  +------------------------+ 00400H   ;;
;;  |     BIOS Data Area     |          ;;
;;  +------------------------+ 00500H   ;;
;;  | PrtScr Status / Unused |          ;;
;;  +------------------------+ 00600H   ;;
;;  |          MBR           |          ;;
;;  +------------------------+ 00800H   ;;
;;  |    MBR stack (2 KB)    |          ;;
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

%define RELOCATED_ADDR 0x600
%define BOOT_ADDR 0x7C00
%define MBR_STACK 0x1000
%define BOOTLOADER_ADDR 0x1000
%define MBR_SIZE 512
%define PARTITION_TABLE_ADDR (RELOCATED_ADDR + 0x1BE)
%define PARTITION_ENTRIES_COUNT 4
%define ACTIVE_PARTITION_FLAG 0x80
%define PASSIVE_PARTITION_FLAG 0x0
%define PARTITION_ENTRY_SIZE 0x10
%define PARTITION_LBA_OFFSET 0x8
%define BOOT_SIGNATURE_ADDR (BOOT_ADDR + 0x1FE)

[BITS 16]
[ORG RELOCATED_ADDR]
entry:
    ; init registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; copying to new address
    mov sp, MBR_STACK
    mov si, BOOT_ADDR
    mov di, RELOCATED_ADDR
    mov cx, MBR_SIZE
    cld
    rep movsb

    ; jump to new location
    jmp 0:relocated

relocated:
    push dx ; store disk number

    mov cx, PARTITION_ENTRIES_COUNT
    mov bx, PARTITION_TABLE_ADDR

boot_partition_loop:
    cmp byte [bx], ACTIVE_PARTITION_FLAG
    je short read_active_bootsector
    cmp byte [bx], PASSIVE_PARTITION_FLAG
    jne invalid_partition_table
    add bx, PARTITION_ENTRY_SIZE
    loop boot_partition_loop

    ; if active partition not found
    sti
    int 0x18

read_active_bootsector:
    sti
check_edd_extensions:
    push bx ; push active partition

    mov ah, 0x41 ; function
    mov bx, 0x55AA
    int 0x13
    jb edd_not_supported
    cmp bx, 0xAA55
    jne edd_not_supported
    test cx, 0x0001
    jz edd_not_supported

; Perform an INT 13 extended read of the first sector of the partition,
; and load it into memory where the MBR was loaded. The extended read
; takes a disk packet that looks like this:
; Offset 0, size 1: Size of packet (16 or 24 bytes).
; Offset 1, size 1: Reserved (0).
; Offset 2, size 2: Number of blocks to transfer.
; Offset 4, size 4: Transfer buffer.
; Offset 8, size 8: Absolute starting sector.
; Offset 0x10, size 8: 64-bit flat address of transfer buffer. Only used
; if the value at offset 4 is 0xFFFFFFFF (which it is not in this case).
;
; Remember that things need to be pushed on in reverse.

read_vbr:
    pop bx ; pop active partition
    pop dx ; pop disk number

    push dword 0x0 ; Push starting sector high.
    push dword [bx + PARTITION_LBA_OFFSET]
    push dword BOOT_ADDR
    push word 0x1 ; Push the sector count.
    push word 0x10 ; Push reserved and packet size.

    clc
    mov ah, 0x42 ; Function 0x42, extended read.
    ;mov dl, dl ; Load the drive number.
    mov si, sp ; SI points to the disk packet address.
    int 0x13 ; Read the sector from the disk.
    add sp, 0x10
    jc read_error

check_bootloader:
    cmp word [BOOT_SIGNATURE_ADDR], 0xAA55
    jne invalid_vbr_signature
    xor dh, dh

    ; jump to VBR code
    jmp 0x0:BOOT_ADDR
;;                          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; End of useful code       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Error handling          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
invalid_partition_table:
    ; error code "0x01"
    mov ax, 0x3130
    call near print_error_and_hlt
edd_not_supported:
    ; error code "0x02"
    mov ax, 0x3230
    call near print_error_and_hlt
read_error:
    ; error code "0x03"
    mov ax, 0x3330
    call near print_error_and_hlt
invalid_vbr_signature:
    ; error code "0x04"
    mov ax, 0x3430
    call near print_error_and_hlt

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Printing function       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
print_error_and_hlt:
    mov word [error_code], ax
    mov si, error_msg
    mov di, 0
    mov ax, 0xB800
    mov es, ax
    cld
print_loop:
    lodsb
    test al, al
    jz short hlt_system
    stosb
    jmp short print_loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Halt system             ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
hlt_system:
    cli
    hlt
    jmp hlt_system
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Data: error messages    ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
error_msg:
db "Error: 0x"
error_code:
dw 0x3030
db 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Developer signature     ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
db "MBR (c) Axon team, 2019", 0
times (510-($-$$)) db 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Boot signature          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
db 0x55, 0xAA
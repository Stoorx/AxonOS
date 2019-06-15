;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Volume Boot Record for FAT32        ;;
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
;;  |          VBR           |          ;;
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
%define VBR_STACK 0x1000
%define BOOTLOADER_ADDR 0x1000
%define VBR_SIZE 512
%define PARTITION_TABLE_ADDR (RELOCATED_ADDR + 0x1BE)
%define PARTITION_ENTRIES_COUNT 4
%define ACTIVE_PARTITION_FLAG 0x80
%define PASSIVE_PARTITION_FLAG 0x0
%define PARTITION_ENTRY_SIZE 0x10
%define PARTITION_LBA_OFFSET 0x8
%define BOOT_SIGNATURE_ADDR (BOOT_ADDR + 0x1FE)

[BITS 16]
[ORG RELOCATED_ADDR]
start:
    jmp short entry
    nop
bsOemName:               DB      "AxonBoot"      ; 0x03

; BPB1:
bpbBytesPerSector:       dw      0               ; 0x0B
bpbSectorsPerCluster:    db      0               ; 0x0D
bpbReservedSectors:      dw      0               ; 0x0E
bpbNumberOfFATs:         db      0               ; 0x10
bpbRootEntries:          dw      0               ; 0x11
bpbTotalSectors:         dw      0               ; 0x13
bpbMedia:                db      0               ; 0x15
bpbSectorsPerFAT:        dw      0               ; 0x16
bpbSectorsPerTrack:      dw      0               ; 0x18
bpbHeadsPerCylinder:     dw      0               ; 0x1A
bpbHiddenSectors:        dd      0               ; 0x1C
bpbTotalSectorsBig:      dd      0               ; 0x20

; BPB2:
bsSectorsPerFAT32:               dd      0               ; 0x24
bsExtendedFlags:                 dw      0               ; 0x28
bsFSVersion:                     dw      0               ; 0x2A
bsRootDirectoryClusterNo:        dd      0               ; 0x2C
bsFSInfoSectorNo:                dw      0               ; 0x30
bsBackupBootSectorNo:            dw      0               ; 0x32
bsreserved:             times 12 db      0               ; 0x34
bsDriveNumber:                   db      0               ; 0x40
bsreserved1:                     db      0               ; 0x41
bsExtendedBootSignature:         db      0               ; 0x42
bsVolumeSerialNumber:            dd      0               ; 0x43
;                                        "123456789AB"
bsVolumeLabel:                   db      "Axon system"   ; 0x47
bsFileSystemName:                db      "FAT32   "      ; 0x52

; Code start
entry:
    ; Context:
    ; AX = 0
    ; ES, DS, SS, CS = 0
    ; DL = drive number
    ; IP = 0x600

    mov     [bsDriveNumber], dl     ; store BIOS boot drive number
    mov bp, sp

calculate_first_data_sector:
    movzx eax, byte [bpbNumberOfFATs]
    mul dword [bsSectorsPerFAT32]
    add eax, word [bpbReservedSectors]
    add eax, dword [bpbHiddenSectors] ; result in eax
    push eax

find_file:
    mov eax, dword [bsRootDirectoryClusterNo]
    mov ebx, [bp - 4] ; first data cluster
    call get_first_sector_of_cluster

    mov di, 0x1000
    mov edx, di

    ; TODO: calculate limit
    movzx ecx, byte [bpbSectorsPerCluster]
    call read_sectors

    mov si, bootloader_name
    mov ecx, 11
    repe cmpsb ; check name
    jcxz file_found
    add di, 32

    ; TODO: check di and make a loop




file_found:
    ; TODO: load file

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Function: get_first_sector_of_cluster   ;;
;;  Args: EAX = cluster number              ;;
;;        EBX = first data sector           ;;
;;  Output: EAX = number of sector          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
get_first_sector_of_cluster:
    sub eax, 2
    mul eax, byte [bpbSectorsPerCluster]
    add ebx
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Function: read_sectors                  ;;
;;  Args: EAX = start sector number         ;;
;;        ES:DI = start address             ;;
;;        ECX = sectors count               ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
read_sectors:
push eax
push edx
push esi
    push dword 0x0 ; Push starting sector high.
    push eax ; Push starting sector low.
    push es ; Buffer segment.
    push di ; Buffer offset.
    push cx ; Push the sector count.
    push word 0x10 ; Push reserved and packet size.

    clc
    mov ah, 0x42 ; Function 0x42, extended read.
    mov dl, byte [bsDriveNumber] ; Load the drive number.
    mov si, sp ; SI points to the disk packet address.
    int 0x13 ; Read the sector from the disk.
    add sp, 0x10
    jc read_error
pop esi
pop edx
pop eax
    ret

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
active_partition_not_found:
    ; error code "0x02"
    mov ax, 0x3230
    call near print_error_and_hlt
edd_not_supported:
    ; error code "0x03"
    mov ax, 0x3330
    call near print_error_and_hlt
read_error:
    ; error code "0x04"
    mov ax, 0x3430
    call near print_error_and_hlt
invalid_vbr_signature:
    ; error code "0x05"
    mov ax, 0x3530
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
;;  Data: bootloader name   ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
bootloader_name:
db "AXONBOOT   "
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Developer signature     ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
db "MBR (c) Axon team, 2019", 0
times (510-($-$$)) db 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Boot signature          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
db 0x55, 0xAA
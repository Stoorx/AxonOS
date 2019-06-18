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
;;  |      Buffer (1 KB)     |          ;;
;;  +------------------------+ 00C00H   ;;
;;  |    MBR stack (1 KB)    |          ;;
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
%define FS_BUFFER 0x800
%define FS_BUFFER_END 0xC00
%define FS_BUFFER_SIZE 0x400
%define FS_BUFFER_SIZE_IN_SECTORS 0x2
%define BOOTLOADER_ADDR 0x1000
%define VBR_SIZE 512
%define PARTITION_TABLE_ADDR (RELOCATED_ADDR + 0x1BE)
%define PARTITION_ENTRIES_COUNT 4
%define ACTIVE_PARTITION_FLAG 0x80
%define PASSIVE_PARTITION_FLAG 0x0
%define PARTITION_ENTRY_SIZE 0x10
%define PARTITION_LBA_OFFSET 0x8
%define BOOT_SIGNATURE_ADDR (BOOT_ADDR + 0x1FE)

%define FIRST_DATA_SECTOR_VAL_ADDRESS (VBR_STACK - 4)
%define FIRST_FAT_SECTOR_VAL_ADDRESS (VBR_STACK - 8)
%define BYTES_PER_CLUSTER_VAL_ADDRESS (VBR_STACK - 12)

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

    mov [bsDriveNumber], dl     ; store BIOS boot drive number
    mov bp, sp
    sub sp, 8 ; reserve space for locals

calculate_first_data_sector:
    movzx ebx, word [bpbReservedSectors]
    add ebx, dword [bpbHiddenSectors]
    mov dword [FIRST_FAT_SECTOR_VAL_ADDRESS], ebx
    movzx eax, byte [bpbNumberOfFATs]
    mul dword [bsSectorsPerFAT32]
    add eax, ebx
    mov dword [FIRST_DATA_SECTOR_VAL_ADDRESS], eax

    movzx eax, byte [bpbSectorsPerCluster]
    mul word [bpbBytesPerSector]
    mov dword [BYTES_PER_CLUSTER_VAL_ADDRESS], eax

find_file:

;; EAX - misc
;; EBX - current cluster
;; ECX - misc, counter
;; EDX - current cluster part

    mov eax, dword [bsRootDirectoryClusterNo]

cluster_chain_loop:
    call get_first_sector_of_cluster
    mov edx, eax ; current sector of cluster is stored in edx

cluster_part_loop:
    mov eax, edx
    mov di, FS_BUFFER
    mov ecx, FS_BUFFER_SIZE_IN_SECTORS
    call read_sectors


    mov eax, FS_BUFFER ; first data record
    file_records_loop:
        mov si, ax
        mov di, bootloader_name
        mov ecx, 11
        repe cmpsb
        jcxz file_found ; if repe reaches equality of strings
        add eax, 32 ; size of file record
        cmp eax, FS_BUFFER_END
        jb file_records_loop ; if we are in buffer

;; POTENTIAL BUG vvv
    add edx, FS_BUFFER_SIZE
    mov eax, edx
    movzx ecx, byte [bpbSectorsPerCluster]
    add eax, ecx ; end of cluster
    cmp edx, eax
    jb cluster_part_loop
;; UNTIL HERE ^^^

; find next cluster
    mov eax, ebx
    add eax, 2
    ; FAT[eax]
    mov ecx, 4 ; fat entry is 4 bytes length
    mul ecx ; in eax offset of FAT entry
    mov ebx, eax
    shr eax, 10
    add eax, dword [FIRST_FAT_SECTOR_VAL_ADDRESS] ; eax contains current fat piece
    and ebx, 0b1111111111 ; ebx contains offset in buffer

    mov di, FS_BUFFER
    mov ecx, FS_BUFFER_SIZE_IN_SECTORS
    call read_sectors

    add ebx, FS_BUFFER
    mov eax, dword [bx] ; fat entry
    and eax, 0x0FFFFFFF
    cmp eax, 0x0FFFFFF8
    jae file_not_found ; if it is the last cluster
    mov ebx, eax
    jmp cluster_chain_loop

file_found:
    ; EAX - file record
    mov ebx, eax
    movzx eax, word [bx + 0x14]
    shl eax, 16
    mov ax, word [bx + 0x1A] ; first cluster in eax

    mov ecx, dword [bx + 0x1C] ; file size in ecx

    mov ebp, BOOTLOADER_ADDR ; current position in ebp

file_read_loop:
    mov ebx, ebp
    shr ebx, 4
    mov es, bx
    mov di, ebp
    and di, 0xF

    mov ebx, eax ; cluster number
    call get_first_sector_of_cluster

    mov edx, ecx ; save file size

    movzx ecx, byte [bpbSectorsPerCluster]
    call read_sectors

    xor eax, eax ; clear SF
    sub edx, ecx
    mov ecx, edx
    js file_read_complete ; if we got negative size (SF is set)

    add ebp, 7
    ; find next cluster

    mov eax, ebx

; TODO!



file_read_complete:


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Function: get_first_sector_of_cluster   ;;
;;  Args: EAX = cluster number              ;;
;;  Output: EAX = number of sector          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
get_first_sector_of_cluster:
    push edx
    sub eax, 2
    mul byte [bpbSectorsPerCluster]
    add eax, dword [FIRST_DATA_SECTOR_VAL_ADDRESS]
    pop edx
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


;;                          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; End of useful code       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Error handling          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
file_not_found:
    ; error code "0x01"
    mov ax, 0x3130
    call near print_error_and_hlt
read_error:
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
db "VBR Error: 0x"
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
db "VBR (c) Axon team, 2019", 0
times (510-($-$$)) db 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Boot signature          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
db 0x55, 0xAA
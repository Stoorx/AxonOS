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
%define CURRENT_FAT_OFFSET_VAL_ADDRESS (VBR_STACK - 16)

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

    mov byte [bsDriveNumber], dl     ; store BIOS boot drive number
    sub sp, 16 ; reserve space for locals

main:
    movzx ebx, word [bpbReservedSectors]
    add ebx, dword [bpbHiddenSectors]
    mov dword [FIRST_FAT_SECTOR_VAL_ADDRESS], ebx ; calculate first fat sector number

    movzx eax, byte [bpbNumberOfFATs]
    mul dword [bsSectorsPerFAT32]
    add eax, ebx
    mov dword [FIRST_DATA_SECTOR_VAL_ADDRESS], eax ; calculate first data sector

    movzx eax, byte [bpbSectorsPerCluster]
    mul word [bpbBytesPerSector]
    mov dword [BYTES_PER_CLUSTER_VAL_ADDRESS], eax ; calculate bytes per cluster number

    xor eax, eax
    dec eax ; 0xFFFFFFFF
    mov dword [CURRENT_FAT_OFFSET_VAL_ADDRESS], eax ; initialize fat frame cache

search_file:
    mov ebx, dword [bsRootDirectoryClusterNo] ; current cluster
    mov eax, ebx ; prepare eax for function call
cluster_chain_iterations:
    call get_first_sector_of_cluster
    mov edx, eax ; edx contains number of current sector
cluster_frame_iterations:
    mov di, FS_BUFFER
    mov cx, FS_BUFFER_SIZE_IN_SECTORS
    call read_sectors

    mov ax, di ; first data record
file_records_iterations:
    mov si, ax
    mov di, bootloader_name
    mov cx, 11
    cld
    repe cmpsb
    je file_found ; if repe reaches equality of strings
    add ax, 32 ; size of file record
    cmp ax, FS_BUFFER_END
    jb short file_records_iterations ; if we are in buffer

    add edx, FS_BUFFER_SIZE_IN_SECTORS ; move edx to next frame
    mov eax, ebx
    inc eax
    call get_first_sector_of_cluster ; find next continuous cluster sector number
    cmp edx, eax
    jae short find_next_cluster_of_directory
    mov eax, edx
    jmp short cluster_frame_iterations
find_next_cluster_of_directory:
    mov eax, ebx
    xor eax, eax
    mov dword [CURRENT_FAT_OFFSET_VAL_ADDRESS], eax ; reset cache
    call get_next_cluster

    cmp eax, 0x0FFFFFF8
    jae file_not_found ; we went through all clusters in chain but have not met the file
    mov ebx, eax
    jmp short cluster_chain_iterations

file_found:
    ; AX - file record
    mov bx, ax
    movzx eax, word [bx + 0x14]
    shl eax, 16
    mov ax, word [bx + 0x1A] ; first cluster in eax

    mov ebp, BOOTLOADER_ADDR ; current position in ebp
    movzx cx, byte [bpbSectorsPerCluster]

file_read_loop:
;EBP - current position in memory
;EAX - current cluster
    mov ebx, ebp
    shr ebx, 4
    mov es, bx
    mov di, bp
    and di, 0xF ; normalize address
    mov ebx, eax ; save cluster number
    call get_first_sector_of_cluster
    call read_sectors
    inc eax
    call get_next_cluster
    cmp eax, 0x0FFFFFF8
    jae file_read_complete
    add ebp, dword [BYTES_PER_CLUSTER_VAL_ADDRESS] ; move ebp to next position
    jmp short file_read_loop

file_read_complete:
    mov dl, byte [bsDriveNumber]
    jmp BOOTLOADER_ADDR

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  HELPER FUNCTIONS                        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Function: get_next_cluster              ;;
;;  Args: EAX = current cluster number      ;;
;;  Return: EAX = next cluster number       ;;
;;  Side effect: changes buffer content     ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
get_next_cluster:
push ebx
push es
push di
    mov ebx, eax
    shr eax, 8 ; eax contains fat frame number

    cmp eax, dword [CURRENT_FAT_OFFSET_VAL_ADDRESS]
    je next

    ; load fat frame
    mov dword [CURRENT_FAT_OFFSET_VAL_ADDRESS], eax ; update cache

    shl eax, 1 ; mul by 2 because we have the buffer of two sectors size
    add eax, dword [FIRST_FAT_SECTOR_VAL_ADDRESS]

    xor bx, bx
    mov es, bx
    mov di, FS_BUFFER
    mov ecx, FS_BUFFER_SIZE_IN_SECTORS
    call read_sectors

    next:
    shl bx, 2 ; mul by 4 (size of fat entry)
    add bx, FS_BUFFER
    mov eax, dword [bx]
    and eax, 0x0FFFFFFF
pop di
pop es
pop ebx
ret


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
;;        CX = sectors count                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
read_sectors:
push eax
push dx
push si
push di
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
pop di
pop si
pop dx
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
db "E:0x"
error_code:
dw 0x3030
db 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Data: bootloader name   ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
bootloader_name:
db "AXONBOOT   "
times (510-($-$$)) db 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Boot signature          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
db 0x55, 0xAA
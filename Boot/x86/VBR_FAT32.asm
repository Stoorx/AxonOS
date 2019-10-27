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
;;  |       VBR (512 B)      |          ;;
;;  +------------------------+ 00800H   ;;
;;  |      VBR II (512 B)    |          ;;
;;  +------------------------+ 00A00H   ;;
;;  |    VBR stack (512 B)   |          ;;
;;  +------------------------+ 00C00H   ;;
;;  |      Buffer (1 KB)     |          ;;
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
%define SECOND_VBR_ADDR 0x800
%define BOOT_ADDR 0x7C00
%define VBR_STACK 0xC00
%define FS_BUFFER 0xC00
%define FS_BUFFER_END 0x1000
%define FS_BUFFER_SIZE 0x400
%define FS_BUFFER_SIZE_IN_SECTORS 0x2
%define BOOTLOADER_ADDR 0x1000
%define VBR_SIZE 512
%define BOOT_SIGNATURE_ADDR (BOOT_ADDR + 0x1FE)

%define FIRST_DATA_SECTOR_VAL_ADDRESS (VBR_STACK - 4)
%define FIRST_FAT_SECTOR_VAL_ADDRESS (VBR_STACK - 8)
%define BYTES_PER_CLUSTER_VAL_ADDRESS (VBR_STACK - 12)
%define CURRENT_FAT_OFFSET_VAL_ADDRESS (VBR_STACK - 16)

[BITS 16]
[ORG RELOCATED_ADDR]
start:
    jmp short entry ; 2b
    nop ; 1b
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
    ; DL = drive number
    ; IP = 0x7C00

    ; init registers
    xor ax, ax ; 2b
    mov ds, ax ; 2b
    mov es, ax ; 2b
    mov ss, ax ; 2b

    ; copying to new address
    mov sp, VBR_STACK ; 3b
    mov si, BOOT_ADDR ; 3b
    mov di, RELOCATED_ADDR ; 3b
    mov cx, VBR_SIZE ; 3b
    cld ; 1b
    rep movsb ; 2b

    ; jump to new location
    jmp 0:relocated ; 5b

relocated:
    mov byte [bsDriveNumber], dl     ; store BIOS boot drive number ; 4b 0x676
    sub sp, 16 ; reserve space for locals ; 3b 0x67a

    ; Clear screen
    mov ax, 0x3 ; 0x67d
    int 0x10 ; 0x680

main:
    ; load VBR II
    mov eax, dword [bpbHiddenSectors] ; 0x682
    inc eax
    mov di, SECOND_VBR_ADDR ; 0x686
    mov cx, 1 ; 0x689
    call read_sectors ; 0x68c -> 0x785

    ; init values
    movzx ebx, word [bpbReservedSectors] ; 6b ; 0x6f8
    add ebx, dword [bpbHiddenSectors] ; 5b ; 0x695
    mov dword [FIRST_FAT_SECTOR_VAL_ADDRESS], ebx ; calculate first fat sector number ; 5b ; 0x69a

    movzx eax, byte [bpbNumberOfFATs] ; 6b ; 0x69f
    mul dword [bsSectorsPerFAT32] ; 5b ; 0x6a5
    add eax, ebx ; 3b ; 0x6aa
    mov dword [FIRST_DATA_SECTOR_VAL_ADDRESS], eax ; calculate first data sector ; 4b ; 0x6ad

    movzx eax, byte [bpbSectorsPerCluster] ; 6b ; 0x6b1
    mul word [bpbBytesPerSector] ; 4b ; 0x6b7
    mov dword [BYTES_PER_CLUSTER_VAL_ADDRESS], eax ; calculate bytes per cluster number ; 4b ; 0x6bb

    xor eax, eax ; 3b ; 0x6bf
    dec eax ; 0xFFFFFFFF ; 2b ; 0x6c2
    mov dword [CURRENT_FAT_OFFSET_VAL_ADDRESS], eax ; initialize fat frame cache ; 4b ; 0x6c4


search_file: ; 0x6c8
    mov ebx, dword [bsRootDirectoryClusterNo] ; current cluster ; 5b ; 0x6c8
    mov eax, ebx ; prepare eax for function call ; 3b ; 0x6cd
cluster_chain_iterations: ; 0x6d0
    call get_first_sector_of_cluster ; 3b ; 0x6d0 -> 0x773
    mov edx, eax ; edx contains number of current sector ; 3b ; 0x6d3
cluster_frame_iterations: ; 0x6d6
    mov di, FS_BUFFER ; 0x6d6
    mov cx, FS_BUFFER_SIZE_IN_SECTORS ; 0x6d9
    call read_sectors ; 0x6dc -> 0x785

    mov ax, di ; first data record ; 0x6df
file_records_iterations: ; 0x6e1
    mov si, ax ; 0x6e1
    mov di, bootloader_name ; 0x6e3
    mov cx, 11 ; 0x6e6
    cld ; 0x6e9
    repe cmpsb ; 0x6ea
    je file_found ; if repe reaches equality of strings ; 0x6ec -> 0x800
    add ax, 32 ; size of file record ; 0x6f0
    cmp ax, FS_BUFFER_END ; 0x6f3
    jb short file_records_iterations ; if we are in buffer ; 0x6f6 -> 0x6e1

    add edx, FS_BUFFER_SIZE_IN_SECTORS ; move edx to next frame ; 0x6f8
    mov eax, ebx ; 0x6fc
    inc eax ; 0x6ff
    call get_first_sector_of_cluster ; find next continuous cluster sector number ; 0x701 -> 0x773
    cmp edx, eax ; 0x704
    jae short find_next_cluster_of_directory ; 0x707 -> 0x70e
    mov eax, edx ; 0x709 // POTENTIAL BUG HERE
    jmp short cluster_frame_iterations ; 0x70c -> 0x6d6
find_next_cluster_of_directory: ; 0x70e
    xor eax, eax
    dec eax ; 0xFFFF'FFFF
    mov dword [CURRENT_FAT_OFFSET_VAL_ADDRESS], eax ; reset cache
    mov eax, ebx
    call get_next_cluster ; 0x718 -> 0x729

    cmp eax, 0x0FFFFFF8 ; 6b ; 0x71b
    jae file_not_found ; we went through all clusters in chain but have not met the file ; 7b ; Does not work! Infinite loop!
    ;mov ax, 0x3535
    ;jmp print_error_and_hlt ;
    mov ebx, eax
    jmp short cluster_chain_iterations



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
push ecx
    push eax
    shr eax, 8 ; eax contains fat frame number

    cmp eax, dword [CURRENT_FAT_OFFSET_VAL_ADDRESS]
    je short next

    ; load fat frame
    mov dword [CURRENT_FAT_OFFSET_VAL_ADDRESS], eax ; update cache

    shl eax, 1 ; mul by 2 because we have the buffer of two sectors size
    add eax, dword [FIRST_FAT_SECTOR_VAL_ADDRESS]

    xor bx, bx
    mov es, bx
    mov di, FS_BUFFER
    mov ecx, FS_BUFFER_SIZE_IN_SECTORS ; 6b !!!
    call read_sectors

    next:
    pop ebx
    shl ebx, 2 ; mul by 4 (size of fat entry)
    add ebx, FS_BUFFER
    mov eax, dword [ebx]
    and eax, 0x0FFFFFFF ; 6b
pop ecx
pop di
pop es
pop ebx
ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Function: get_first_sector_of_cluster   ;;
;;  Args: EAX = cluster number              ;;
;;  Output: EAX = number of sector          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
get_first_sector_of_cluster: ; 0x773
push edx ; 0x773
    sub eax, 2 ; 0x775
    mul byte [bpbSectorsPerCluster] ; 0x779
    add eax, dword [FIRST_DATA_SECTOR_VAL_ADDRESS] ; 0x77d
pop edx ; 0x782
ret ; 0x784

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Function: read_sectors                  ;;
;;  Args: EAX = start sector number         ;;
;;        ES:DI = start address             ;;
;;        CX = sectors count                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
read_sectors: ; 0x785
push eax ; 0x785
push dx ; 0x787
push si ; 0x788
push di ; 0x789
    push dword 0x0 ; Push starting sector high. ; 0x78a
    push eax ; Push starting sector low. ; 0x78d
    push es ; Buffer segment. ; 0x78f
    push di ; Buffer offset. ; 0x790
    push cx ; Push the sector count. ; 0x791
    push word 0x10 ; Push reserved and packet size. ; 0x792

    clc ; 0x794
    mov ah, 0x42 ; Function 0x42, extended read. ; 0x795
    mov dl, byte [bsDriveNumber] ; Load the drive number. ; 0x797
    mov si, sp ; SI points to the disk packet address. ; 0x79b
    int 0x13 ; Read the sector from the disk. ; 0x79d
    add sp, 0x10 ; 0x79f
    jc short read_error ; 0x7a2 -> 0x7b0
pop di ; 0x7a4
pop si ; 0x7a5
pop dx ; 0x7a6
pop eax ; 0x7a7
ret ; 0x7a9


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
    jmp near print_error_and_hlt
read_error:
    ; error code "0x02"
    mov ax, 0x3230
;    call near print_error_and_hlt
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
    jmp short hlt_system
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Data: error messages    ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
error_msg:
db "VBR Error: 0x"
error_code:
dw "00"
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

file_found:
    ; AX - file record
    mov bx, ax
    movzx eax, word [bx + 0x14]
    shl eax, 16
    mov ax, word [bx + 0x1A] ; first cluster in eax

    mov ebp, BOOTLOADER_ADDR ; current position in ebp ; 6b
    movzx cx, byte [bpbSectorsPerCluster] ; 5b

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
    mov eax, ebx ; restore cluster number
    call get_next_cluster
    cmp eax, 0x0FFFFFF8 ; 6b
    jae file_read_complete
    add ebp, dword [BYTES_PER_CLUSTER_VAL_ADDRESS] ; move ebp to next position ; 5b
    jmp short file_read_loop

file_read_complete:
    mov dl, byte [bsDriveNumber]
    jmp BOOTLOADER_ADDR

times (1024-($-$$)) db 0
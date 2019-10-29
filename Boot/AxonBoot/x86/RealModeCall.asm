%define C_EAX 0
%define C_EBX 4
%define C_ECX 8
%define C_EDX 12

%define C_ESI 16
%define C_EDI 20

%define C_EFLAGS 24

%define C_DS 28
%define C_ES 32
%define C_FS 36
%define C_GS 40

%define SIZEOF_CTX 44

%define A_CTX 8
%define A_INT_NUM 12

[SECTION .text]
[BITS 32]
global realModeIntExecute
realModeIntExecute:
    push ebp
    mov ebp, esp

    ; Save non-volatile registers
    push ebx
    push esi
    push edi
    pushf

    cli

    sub esp, 0x8
    sidt [esp] ; Save IDT

    sub esp, 0x8
    sgdt [esp] ; Save GDT

    lidt [rmIdtr] ; Load RM IDT (IVT)
    lgdt [rmGdtr] ; Load RM GDT

    ; Go to 16 bit PM
    jmp 0x8:pm16Bit

pmRestore:
    ; Reload selectors
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    ; Return 0
    xor eax, eax

    ; Restore non-volatile registers
    popf
    pop edi
    pop esi
    pop ebx

    pop ebp
    ret

[SECTION .text16]
[BITS 16]
global pm16Bit
pm16Bit:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    ; Turn off Protected mode
    mov eax, cr0
    and eax, 0xFFFFFFFE
    mov cr0, eax

    mov ax, 0x0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    ; Jump to RM
    jmp 0x0:.rmEntry

.rmEntry:
    push dword ebp ; Save ebp (some BIOSes affects ebp during the call)
    ; Push int return address
    pushf
    push cs
    push .intRet

    ; Emulate int with iret
    mov ebx, dword [ebp + A_INT_NUM]
    shl bx, 4 ; get address of interrupt vector
    pushf
    mov ax, [bx + 2] ; vector segment
    push ax
    mov ax, [bx]
    push ax

    ; Load RM context
    mov ebx, dword [ebp + A_CTX]

    mov eax, dword [ebx + C_DS]
    mov ds, ax
    mov eax, dword [ebx + C_ES]
    mov es, ax
    mov eax, dword [ebx + C_FS]
    mov fs, ax
    mov eax, dword [ebx + C_GS]
    mov gs, ax

    mov esi, dword [ebx + C_ESI]
    mov edi, dword [ebx + C_EDI]

    push word [ebx + C_EFLAGS]
    popf

    mov ecx, dword [ebx + C_ECX]
    mov edx, dword [ebx + C_EDX]
    mov eax, dword [ebx + C_EAX]
    mov ebx, dword [ebx + C_EBX]

    iret ; execute interrupt
.intRet:
    cli

    ; Restore ebp after call
    pop dword ebp

    ; Save context
    push eax
    push ebx
    push ecx
    push edx

    mov ebx, [ebp + A_CTX]

    mov dword [ebx + C_ESI], esi
    mov dword [ebx + C_EDI], edi

    xor eax, eax
    pushf
    pop ax
    mov dword [ebx + C_EFLAGS], eax

    xor eax, eax

    mov ax, ds
    mov dword [ebx + C_DS], eax
    mov ax, es
    mov dword [ebx + C_ES], eax
    mov ax, fs
    mov dword [ebx + C_FS], eax
    mov ax, gs
    mov dword [ebx + C_GS], eax

    pop eax
    mov dword [ebx + C_EDX], eax
    pop eax
    mov dword [ebx + C_ECX], eax
    pop eax
    mov dword [ebx + C_EBX], eax
    pop eax
    mov dword [ebx + C_EAX], eax

    ; Enter Protected Mode
    lgdt [esp]
    add esp, 8
    lidt [esp]
    add esp, 8

    ; Set PM flag
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp 0x8:pmRestore

[SECTION .data16]
align 8
rmGdt:
    dq 0 ; null
    db 0FFh,0FFh,0,0,0,9Ah,08Fh,0 ; code
    db 0FFh,0FFh,0,0,0,92h,08Fh,0 ; data
rmGdtr:
    dw rmGdtr - rmGdt - 1
    dd rmGdt
rmIdtr:
    dw 0x3FF
    dd 0



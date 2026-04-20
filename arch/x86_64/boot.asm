global start
extern kernel_main

section .text
bits 64

start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rsp, stack_top

    call clear_bss
    call kernel_main

    cli
    hlt

clear_bss:
    mov rdi, bss_start
    mov rcx, bss_end
    sub rcx, rdi
    xor rax, rax
    rep stosb
    ret

section .bss
align 16
stack_bottom:
    resb 16386
stack_top:
bss_start:
bss_end:
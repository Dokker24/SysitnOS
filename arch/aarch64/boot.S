
.section .text.boot
.global _start

_start:
    mov x19, x0
    mov x20, x1

    ldr x1, =_stack_top
    mov sp, x1

    ldr x1, =__bss_start
    ldr x2, =__bss_end
    sub x2, x2, x1
    cbz x2, 4f
    
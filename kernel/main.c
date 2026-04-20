#include "boot/common/boot_info.h"

void kernel_main(BootInfo *info) {
    while (1) {
        __asm__ volatile ("nop");
    }
}
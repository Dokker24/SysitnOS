#pragma once

#include <stdint.h>

void arch_init(void);
void arch_halt(void) __attribute__((noreturn));
void arch_enable_interrupts(void);

typedef uint64_t PhysAddr;
typedef uint64_t VirtAddr;
typedef void* PageTable;

PageTable arch_create_page_table(void);
void arch_map_page(PageTable pt, VirtAddr virt, PhysAddr phys, uint32_t flags);
void arch_switch_page_table(PageTable pt);

#define PAGE_SIZE 4096
#define PAGE_PRESENT (1 << 0)
#define PAGE_WRITE (1 << 1)
#define PAGE_USER (1 << 2)

void arch_uart_putc(char c);
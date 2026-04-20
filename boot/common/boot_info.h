#pragma once

#include <stdint.h>
#include <stddef.h>

typedef enum {
    MEM_USABLE = 0,
    MEM_RESERVED = 1,
    MEM_ACPI = 2,
    MEM_FRAMEBUF = 3,
    MEM_KERNEL = 4,
} MemoryRegionType;

typedef struct {
    uint64_t base;
    uint64_t size;
    MemoryRegionType type;
} MemoryRegion;

#define MAX_MEMORY_REGIONS 256

typedef struct {
    MemoryRegion regions[MAX_MEMORY_REGIONS];
    size_t region_count;

    uint64_t fb_addr;
    uint32_t fb_width, fb_height, fb_pitch;

    void *acpi_rsdp, *fdt_blob;

    const char *cmdline;
} BootInfo;
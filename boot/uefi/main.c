#include <efi/efi.h>
#include "boot/common/boot_info.h"

extern void kernel_main(BootInfo *info);

static BootInfo g_boot_info;

static void collect_memory_map(EFI_SYSTEM_TABLE *ST) {
    UINTN map_size = 0, map_key, desc_size;
    UINT32 desc_ver;
    EFI_MEMORY_DESCRIPTOR *map = NULL;

    ST -> BootServices -> GetMemoryMap(&map_size, map, &map_key, &desc_size, &desc_ver);
    map_size += 2 * desc_size;
    ST -> BootServices -> AllocatePool(EfiLoaderData, map_size, (void **) &map);
    ST -> BootServices -> GetMemoryMap(&map_size, map, &map_key, &desc_size, &desc_ver);

    size_t n = 0;
    for (UINTN i = 0; i < map_size / desc_size && n < MAX_MEMORY_REGIONS; i++) {
        EFI_MEMORY_DESCRIPTOR *d = (void *) map + i * desc_size;
        MemoryRegionType t = MEM_RESERVED;

        switch (d -> Type) {
            case EfiConventionalMemory: {
                t = MEM_USABLE;
                break;
            }
            case EfiACPIReclaimMemory: {
                t = MEM_ACPI;
                break;
            }
            case EfiLoaderCode:
            case EfiLoaderData: {
                t = MEM_KERNEL;
                break;
            }
            default: break;
        }

        g_boot_info.regions[n++] = (MemoryRegion) {
            .base = d -> PhysicalStart,
            .size = d -> NumberOfPages * 4096,
            .type = t,
        };
    }
    g_boot_info.region_count = n;
    ST -> BootServices -> ExitBootServices(NULL, map_key);
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *ST) {
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;
    ST -> BootServices -> LocateProtocol(&gop_guid, NULL, (void **) &gop);

    if (gop) {
        g_boot_info.fb_addr = gop -> Mode -> FrameBufferBase;
        g_boot_info.fb_width = gop -> Mode -> Info -> HorizontalResolution;
        g_boot_info.fb_height = gop -> Mode -> Info -> VerticalResolution;
        g_boot_info.fb_pitch = gop -> Mode -> Info -> PixelsPerScanLine * 4;
    }

    collect_memory_map(ST);
    kernel_main(&g_boot_info);

    return EFI_SUCCESS;
}
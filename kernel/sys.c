#include "sys.h"
#include "util.h"
#pragma pack (1)
__attribute__((aligned(4096)))
struct {
  struct gdt_entry null;
  struct gdt_entry kernel_code;
  struct gdt_entry kernel_data;
  struct gdt_entry null2;
  struct gdt_entry user_data;
  struct gdt_entry user_code;
  struct gdt_entry ovmf_data;
  struct gdt_entry ovmf_code;
  struct gdt_entry tss_low;
  struct gdt_entry tss_high;
} gdt_table = {
    {0, 0, 0, 0x00, 0x00, 0},  /* 0x00 null  */
    {0, 0, 0, 0x9a, 0xa0, 0},  /* 0x08 kernel code (kernel base selector) */
    {0, 0, 0, 0x92, 0xa0, 0},  /* 0x10 kernel data */
    {0, 0, 0, 0x00, 0x00, 0},  /* 0x18 null (user base selector) */
    {0, 0, 0, 0x92, 0xa0, 0},  /* 0x20 user data */
    {0, 0, 0, 0x9a, 0xa0, 0},  /* 0x28 user code */
    {0, 0, 0, 0x92, 0xa0, 0},  /* 0x30 ovmf data */
    {0, 0, 0, 0x9a, 0xa0, 0},  /* 0x38 ovmf code */
    {0, 0, 0, 0x89, 0xa0, 0},  /* 0x40 tss low */
    {0, 0, 0, 0x00, 0x00, 0},  /* 0x48 tss high */
};
#pragma pack()

static const CHAR16 hex[] = {
    u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7',
    u'8', u'9', u'A', u'B', u'C', u'D', u'E', u'F',
};

const CHAR16 *memory_types[] = {
    L"EfiReservedMemoryType",
    L"EfiLoaderCode",
    L"EfiLoaderData",
    L"EfiBootServicesCode",
    L"EfiBootServicesData",
    L"EfiRuntimeServicesCode",
    L"EfiRuntimeServicesData",
    L"EfiConventionalMemory",
    L"EfiUnusableMemory",
    L"EfiACPIReclaimMemory",
    L"EfiACPIMemoryNVS",
    L"EfiMemoryMappedIO",
    L"EfiMemoryMappedIOPortSpace",
    L"EfiPalCode",
};

const CHAR16 *memory_type_to_str(UINT32 type) {
  if (type > sizeof(memory_types) / sizeof(CHAR16 *))
    return L"Unknown";
  else
    return memory_types[type];
}

static CHAR16 buff[512];
static UINT16 buff_pos = 0;

#define CHECK_POS             \
  do {                        \
    if (buff_pos >= 511) {    \
      buff[511] = 0;          \
      write_to_console(buff); \
      buff_pos = 0;           \
    }                         \
  } while (0);

static void write_to_console(CHAR16 *str) {
  uefi_call_wrapper(global_env->st->ConOut->OutputString, 2, global_env->st->ConOut, str);
}

static void putchar(const CHAR16 ch) {
  CHECK_POS;
  buff[buff_pos++] = ch;
}

void clean_log_cache() {
  putchar('\0');
  write_to_console(buff);
}

void g_print(CHAR16 *str, ...) {
  va_list args;
  va_start(args, str);
  while (*str) {
#define NUM_CASE(type)                                                        \
  do {                                                                        \
    type p = va_arg(args, type);                                              \
    if (p < 0) {                                                              \
      putchar('-');                                                           \
      p *= -1;                                                                \
    }                                                                         \
    type tmp = p;                                                             \
    uint64_t len = 1;                                                         \
    while ((tmp /= 10))                                                       \
      ++len;                                                                  \
    buff_pos += len;                                                          \
    CHECK_POS;                                                                \
    uint64_t j = buff_pos;                                                    \
    while (len) {                                                             \
      buff[--j] = u'0' + (p % 10);                                            \
      p /= 10;                                                                \
      len--;                                                                  \
    }                                                                         \
  } while (0);
    const CHAR16 ch = *str;
    str++;
    if (ch == u'%') {
      const CHAR16 next_ch = *str;
      str++;
      if (next_ch == u's') {
        // %s
        CHAR16 *p = va_arg(args, CHAR16*);
        while (*p) {
          putchar(*p);
          p++;
        }
      } else if (next_ch == u'p') {
        uintptr_t p = (uintptr_t) va_arg(args, uintptr_t*);
        putchar(u'0');
        putchar(u'x');
        for (uint32_t i = 0; i < 16; i++)
          putchar(hex[p >> (60 - (i << 2)) & 15]);
      } else if (next_ch == u'i' || next_ch == u'd') {
        NUM_CASE(int32_t);
      } else if (next_ch == u'u') {
        NUM_CASE(uint32_t);
      } else if (next_ch == u'l') {
        const CHAR16 nnext_ch = *str;
        str++;
        if (nnext_ch == u'i' || nnext_ch == u'd') {
          NUM_CASE(int64_t);
        } else if (nnext_ch == u'u') {
          NUM_CASE(uint64_t);
        }
      }
    } else {
      putchar(ch);
    }
#undef NUM_CASE
  } // end while
  va_end(args);
}

void find_acpi2() {
  const EFI_GUID expected_table_guid = ACPI_20_TABLE_GUID;
  EFI_CONFIGURATION_TABLE *configuration_tables = global_env->st->ConfigurationTable;
  uint64_t length = global_env->st->NumberOfTableEntries;
  for (uint64_t i = 0; i < length; i++) {
    const EFI_CONFIGURATION_TABLE table = configuration_tables[i];
    const EFI_GUID table_guid = configuration_tables[i].VendorGuid;
    if (CompareMem(&table_guid, &expected_table_guid, sizeof(EFI_GUID)) == 0) {
      g_print(L"debug: address: %p \r\n", table.VendorTable);
    }
  }
}

static EFI_STATUS memory_map(EFI_MEMORY_DESCRIPTOR **map_buf,
                             UINTN *map_size,
                             UINTN *map_key,
                             UINTN *desc_size,
                             UINT32 *desc_version) {
  EFI_STATUS err = EFI_SUCCESS;
  *map_size = sizeof(**map_buf) * 31;
  get_map:
  *map_size += sizeof(**map_buf);
  err = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, *map_size, (void **) map_buf);
  if (err != EFI_SUCCESS) {
    Print(L"ERROR: Failed to allocate pool for memory map");
    return err;
  }

  err = uefi_call_wrapper(BS->GetMemoryMap, 5, map_size, *map_buf, map_key, desc_size, desc_version);
  if (err != EFI_SUCCESS) {
    if (err == EFI_BUFFER_TOO_SMALL) {
      uefi_call_wrapper(BS->FreePool, 1, (void *) *map_buf);
      goto get_map;
    }
    Print(L"ERROR: Failed to get memory map");
  }
  return err;
}

static void print_memory_map() {
  Print(L"Memory Map Size: %d\r\n", global_env->uefi_mmap_original.nBytes);
  Print(L"Map Key: %d\r\n", global_env->uefi_mmap_original.mapKey);
  Print(L"Descriptor Version: %d\r\n", global_env->uefi_mmap_original.descriptor_version);
  Print(L"Descriptor Size: %d\r\n\r\n", global_env->uefi_mmap_original.descriptor_size);
  EFI_MEMORY_DESCRIPTOR *desc = global_env->uefi_mmap_original.buffer;
  uint64_t i = 0;
  while ((void *) desc < (void *) global_env->uefi_mmap_original.buffer + global_env->uefi_mmap_original.nBytes) {
    uint64_t mapping_size = desc->NumberOfPages * PAGE_SIZE;
    Print(L"[#%02d] Type: %s  Attr: 0x%x\r\n", i, memory_type_to_str(desc->Type), desc->Attribute);
    Print(L"      Phys: %016llx-%016llx\r\n", desc->PhysicalStart, desc->PhysicalStart + mapping_size);
    Print(L"      Virt: %016llx-%016llx\r\n\r\n", desc->VirtualStart, desc->VirtualStart + mapping_size);
    desc = NextMemoryDescriptor(desc, global_env->uefi_mmap_original.descriptor_size);
    i++;
  }
}

void setup_uefi() {
  EFI_STATUS status = memory_map(&global_env->uefi_mmap_original.buffer,
                                 &global_env->uefi_mmap_original.nBytes,
                                 &global_env->uefi_mmap_original.mapKey,
                                 &global_env->uefi_mmap_original.descriptor_size,
                                 &global_env->uefi_mmap_original.descriptor_version);
  if (EFI_ERROR(status)) {
    g_print(L"unexpected error\r\n");
  }
#if defined(_DEBUG)
  print_memory_map();
#endif
}

void load_gdt(table_ptr *gdt_ptr) {
  asm volatile("cli");
  asm volatile ("lgdt %0" : : "m" (*gdt_ptr));
}

void setup_gdt() {
  // todo
}

#undef CHECK_POS

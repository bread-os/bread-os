#pragma once
#include "util.h"

// static uint64_t next_alloc_page = 0;

typedef struct uefi_mmap {
  uint64_t nBytes;
  EFI_MEMORY_DESCRIPTOR *buffer;
  uint64_t mapKey;
  uint64_t descriptor_size;
  uint32_t descriptor_version;
} uefi_mmap;

typedef struct {
  EFI_HANDLE handle;
  EFI_SYSTEM_TABLE *st;
  uefi_mmap uefi_mmap_original;
} environment;

static environment *global_env;

struct IDT {
  uint16_t limit;
  uint64_t offset;
} __attribute__((packed));
typedef struct IDT IDT;

struct IDTDescriptor {
  uint16_t offset_1; // offset bits 0..15
  uint16_t selector; // a code segment selector in GDT or LDT
  uint16_t flags;       // various flags, namely:
  // 0..2: Interrupt stack table
  // 3..7: zero
  // 8..11: type
  // 12: zero
  // 13..14: descriptor privilege level
  // 15: segment present flag
  uint16_t offset_2; // offset bits 16..31
  uint32_t offset_3; // offset bits 32..63
  uint32_t zero;     // reserved
} __attribute__((packed));
typedef struct IDTDescriptor IDTDescriptor;

#pragma pack (1)
struct gdt_entry {
  uint16_t limit15_0;
  uint16_t base15_0;
  uint8_t base23_16;
  uint8_t type;
  uint8_t limit19_16_and_flags;
  uint8_t base31_24;
};

struct tss {
  uint32_t reserved0;
  uint64_t rsp0;
  uint64_t rsp1;
  uint64_t rsp2;
  uint64_t reserved1;
  uint64_t ist1;
  uint64_t ist2;
  uint64_t ist3;
  uint64_t ist4;
  uint64_t ist5;
  uint64_t ist6;
  uint64_t ist7;
  uint64_t reserved2;
  uint16_t reserved3;
  uint16_t iopb_offset;
} tss;

typedef struct table_ptr {
  uint16_t limit;
  uint64_t base;
} table_ptr;
#pragma pack ()

void g_print(CHAR16 *str, ...);
void clean_log_cache();

void init_interrupts();
void find_acpi2();
void setup_uefi();

void load_gdt(table_ptr *gdt_ptr);
void setup_gdt();

void g_print(CHAR16 *str, ...);

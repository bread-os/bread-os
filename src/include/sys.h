#ifndef BREAD_OS_SRC_KERNEL_SYS_H_
#define BREAD_OS_SRC_KERNEL_SYS_H_
#include <Uefi.h>
#include "util.h"

typedef struct {
  EFI_SYSTEM_TABLE *st;
} environment;
static environment *global_env;

void gdb_log(CHAR16 *str, ...);

EFI_STATUS init_interrupts();

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

#endif //BREAD_OS_SRC_KERNEL_SYS_H_

#ifndef BREAD_OS_SRC_KERNEL_SYS_H_
#define BREAD_OS_SRC_KERNEL_SYS_H_
#include <Uefi.h>
#include "util.h"

typedef struct {
  EFI_SYSTEM_TABLE *st;
} environment;
static environment *global_env;

void gdb_log(CHAR16 *str, ...);

struct IDT {
  uint16_t limit;
  uint64_t offset;
} __attribute__((packed));

struct IDTDescriptor {
  uint16_t offset_1; // offset bits 0..15
  uint16_t selector; // a code segment selector in GDT or LDT

  uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
  uint8_t type_attr; // type and attributes

  uint16_t offset_2; // offset bits 16..31
  uint32_t offset_3; // offset bits 32..63
  uint32_t zero;     // reserved
} __attribute__((packed));

#endif //BREAD_OS_SRC_KERNEL_SYS_H_

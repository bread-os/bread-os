#include "sys.h"
#include "util.h"

static const CHAR16 hex[] = {
    u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7',
    u'8', u'9', u'A', u'B', u'C', u'D', u'E', u'F',
};

static CHAR16 *buffer_memory = NULL;

void gdb_log(CHAR16 *str, ...) {
  if (buffer_memory == NULL) {
    CHECK_STATUS(global_env->st->BootServices->AllocatePages(AllocateAnyPages,
                                                             EfiBootServicesData,
                                                             1,
                                                             (EFI_PHYSICAL_ADDRESS *) &buffer_memory));
  }
  va_list args;
  UINT16 pos = 0;
  va_start(args, str);
  while (*str) {
    // todo
  }
  va_end(args);
}

void unknown_handler();
void int8_handler();

EFI_STATUS init_interrupts() {
  IDT idt = {0};
  __asm__("sidt %0":"=m"(idt));
  IDTDescriptor *idt_off = (struct IDTDescriptor *) idt.offset;

  IDTDescriptor unknown_descriptor = {
      .offset_1 = (uint64_t) unknown_handler & 0xFFFF,
      .offset_2 = (((uint64_t) unknown_handler) >> 16) & 0xFFFF,
      .offset_3 = (((uint64_t) unknown_handler) >> 32),
      .selector = 0x28,
      .zero = 0,
      .flags = 0x8E00
  };

  for (int i = 32; i < 256; i += 1) {
    idt_off[i] = unknown_descriptor;
  }

  // FIXME: we currently use int8 handler for all exception interrupts.
  for (int i = 0; i < 20; i += 1) {
    if (i == 9) continue;
    idt_off[i].offset_1 = (uint64_t) int8_handler & 0xFFFF;
    idt_off[i].offset_2 = (((uint64_t) int8_handler) >> 16) & 0xFFFF;
    idt_off[i].offset_3 = (((uint64_t) int8_handler) >> 32);
    idt_off[i].selector = 0x28;
    idt_off[i].flags = 0x8E00;
  }

  return EFI_SUCCESS;
}

__attribute__((naked)) void unknown_handler() {

}

__attribute__((naked)) void int8_handler() {

}

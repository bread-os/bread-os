#include "sys.h"
#include "util.h"

static const CHAR16 hex[] = {
    u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7',
    u'8', u'9', u'A', u'B', u'C', u'D', u'E', u'F',
};

static CHAR16 buff[512];
static UINT16 buff_pos = 0;

#define CHECK_POS             \
  do {                  \
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
  do {                                                                 \
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

#undef CHECK_POS

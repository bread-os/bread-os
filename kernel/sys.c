#include "sys.h"
#include "util.h"

static const CHAR16 hex[] = {
    u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7',
    u'8', u'9', u'A', u'B', u'C', u'D', u'E', u'F',
};

static CHAR16 buff[512];
static UINT16 buff_pos = 0;

void write_to_console(CHAR16 *str) {
  uefi_call_wrapper(global_env->st->ConOut->OutputString, 2, global_env->st->ConOut, str);
}
void clean_log_cache() {
  write_to_console(buff);
}

void gdb_log(CHAR16 *str, ...) {
  va_list args;
  va_start(args, str);
  UINT16 pos = buff_pos;
  while (*str) {
#define CHECK_POS             \
  while(0) {                  \
    if (pos >= 511) {         \
      buff[pos] = 0;          \
      write_to_console(buff); \
      pos = 0;                \
    }                         \
  }

#define NUM_CASE(_type)                                                        \
  while (0) {                                                                  \
    if (pos >= 511) {                                                          \
      buff[pos] = 0;                                                           \
      write_to_console(buff);                                                  \
      pos = 0;                                                                 \
    }                                                                          \
    _type p = va_arg(args, _type);                                             \
    if (p < 0) {                                                               \
      buff[pos++] = u'-';                                                      \
      p = p * (-1);                                                            \
    }                                                                          \
    _type tmp = p;                                                             \
    uint32_t i = 1;                                                            \
    while ((tmp /= 10))                                                        \
      ++i;                                                                     \
    pos += i;                                                                  \
    CHECK_POS;                                                                 \
    uint32_t j = pos;                                                          \
    while (i) {                                                                \
      tmp = p;                                                                 \
      p /= 10;                                                                 \
      tmp -= (p * 10);                                                         \
      buff[--j] = u'0' + tmp;                                                  \
      --i;                                                                     \
    }                                                                          \
    break;                                                                     \
  }
    CHECK_POS;
    const CHAR16 ch = *str;
    if (ch == u'%') {
      const CHAR16 next_ch = *str;
      if (next_ch == u's') {
        // %s
        CHAR16 *p = va_arg(args, CHAR16*);
        while (p) {
          CHECK_POS;
          buff[pos++] = *p;
          p++;
        }
      } else if (next_ch == u'p') {
        uintptr_t p = (uintptr_t) va_arg(args, uintptr_t*);
        buff[pos++] = '0';
        CHECK_POS;
        buff[pos++] = 'x';
        for (uint32_t i = 0; i < 16; i++) {
          CHECK_POS;
          buff[pos++] = hex[p >> (60 - (i << 2)) & 15];
        }
      } else if (next_ch == u'i') {
        NUM_CASE(int32_t);
      } else if (next_ch == u'u') {
        NUM_CASE(uint32_t);
      } else if (next_ch == u'l') {
        const CHAR16 nnext_ch = *str;
        str++;
        if (nnext_ch == 'i') {
          NUM_CASE(int64_t);
        } else if (nnext_ch == 'u') {
          NUM_CASE(uint64_t);
        }
      }
    } else {
      buff[pos] = *str;
      pos++;
    }
#undef NUM_CASE
#undef CHECK_POS
    str++;
  } // end while
  va_end(args);
  buff_pos = pos;
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

#include "sys.h"
#include "util.h"
static const CHAR16 hex[] = {
    u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7',
    u'8', u'9', u'A', u'B', u'C', u'D', u'E', u'F',
};

static CHAR16 *buffer_memory = NULL;

void gdb_log(CHAR16 *str, ...) {
#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_copy(dest, src) __builtin_va_copy(dest, src)
  typedef __builtin_va_list va_list;
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

#undef va_copy
#undef va_arg
#undef va_end
#undef va_start
}

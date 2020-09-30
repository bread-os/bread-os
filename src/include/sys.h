#ifndef BREAD_OS_SRC_KERNEL_SYS_H_
#define BREAD_OS_SRC_KERNEL_SYS_H_
#include <Uefi.h>
typedef struct {
  EFI_SYSTEM_TABLE *st;
} environment;
static environment *global_env;

void gdb_log(CHAR16 *str, ...);

#endif //BREAD_OS_SRC_KERNEL_SYS_H_

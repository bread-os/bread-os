#ifndef BREAD_OS_SRC_INCLUDE_UTIL_H_
#define BREAD_OS_SRC_INCLUDE_UTIL_H_
#include <Uefi.h>

#define CHECK_STATUS_MESSAGE(exp, message)                                                \
  do {                                                                                    \
    EFI_STATUS status;                                                                    \
    status = (exp);                                                                       \
    if (EFI_ERROR(status)) {                                                              \
      global_env->st->ConOut->OutputString(global_env->st->ConOut, (CHAR16 *) message);   \
    }                                                                                     \
  } while (0);

#define CHECK_STATUS(exp) CHECK_STATUS_MESSAGE(exp, "unknown error")

#endif //BREAD_OS_SRC_INCLUDE_UTIL_H_

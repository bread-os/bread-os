#pragma once
#include <efi.h>
#include <efilib.h>

#define CHECK_STATUS_MESSAGE(exp, message)                                                \
  do {                                                                                    \
    EFI_STATUS __status;                                                                  \
    __status = (exp);                                                                     \
    if (EFI_ERROR(__status)) {                                                            \
      global_env->st->ConOut->OutputString(global_env->st->ConOut, (CHAR16 *) message);   \
    }                                                                                     \
  } while (0);

#define CHECK_STATUS(exp) CHECK_STATUS_MESSAGE(exp, "unknown error")

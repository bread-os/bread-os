#include <efi.h>
#include <efilib.h>
#include "sys.h"

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  global_env->st = SystemTable;
#if defined(_GNU_EFI)
  InitializeLib(ImageHandle, SystemTable);
#endif
  EFI_STATUS Status;
  EFI_INPUT_KEY Key;

  uefi_call_wrapper(global_env->st->ConOut->Reset, 2, global_env->st->ConOut, FALSE);
  gdb_log(L"Hello, world!\r\n");
  gdb_log(L"This is second line");
  gdb_log(L".");
  clean_log_cache();

  UINTN Event;
  uefi_call_wrapper(global_env->st->ConIn->Reset, 2, global_env->st->ConIn, FALSE);
  uefi_call_wrapper(global_env->st->BootServices->WaitForEvent, 3, 1, &SystemTable->ConIn->WaitForKey, &Event);
#if defined(_DEBUG)
  // If running in debug mode, use the EFI shut down call to close QEMU
  uefi_call_wrapper(global_env->st->RuntimeServices->ResetSystem, 4, EfiResetShutdown, EFI_SUCCESS, 0, NULL);
#endif
  return Status;
}

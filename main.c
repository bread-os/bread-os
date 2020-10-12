#include <efi.h>
#include <efilib.h>
#include "sys.h"

const static CHAR16 *os_name = L"bread-os";
// 536237 in decimal
const static int64_t magic_number = 0x82EAD;

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  global_env->st = SystemTable;
#if defined(_GNU_EFI)
  InitializeLib(ImageHandle, SystemTable);
#endif

  uefi_call_wrapper(global_env->st->ConOut->Reset, 2, global_env->st->ConOut, FALSE);
  g_print(L"OS name: %s\r\n", os_name);
  g_print(L"os name address: %lu\r\n", magic_number);
  g_print(L"Hello, world!\r\n");
  g_print(L"This is second line");
  g_print(L".\r\n");
  find_acpi2();
  setup_uefi();
  clean_log_cache();

  UINTN Event;
  uefi_call_wrapper(global_env->st->ConIn->Reset, 2, global_env->st->ConIn, FALSE);
  uefi_call_wrapper(global_env->st->BootServices->WaitForEvent, 3, 1, &SystemTable->ConIn->WaitForKey, &Event);
#if defined(_DEBUG)
  // If running in debug mode, use the EFI shut down call to close QEMU
  uefi_call_wrapper(global_env->st->RuntimeServices->ResetSystem, 4, EfiResetShutdown, EFI_SUCCESS, 0, NULL);
#endif
  return 0;
}

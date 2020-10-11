#include <efi.h>
#include <efilib.h>
#include "sys.h"

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  global_env->st = SystemTable;
  EFI_STATUS Status;
  EFI_INPUT_KEY Key;

  gdb_log(L"Hello, world!\r\n");
  gdb_log(L"This is second line");
  gdb_log(L".");
  clean_log_cache();

  Status = SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
  if (EFI_ERROR(Status))
    return Status;

  while ((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY);

  return Status;
}

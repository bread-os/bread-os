#include <Uefi.h>
#include "sys.h"

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  global_env->st = SystemTable;
  EFI_STATUS Status;
  EFI_INPUT_KEY Key;

  Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16 *) L"Hello World\n\r");
  if (EFI_ERROR(Status))
    return Status;

  Status = SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
  if (EFI_ERROR(Status))
    return Status;

  while ((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY);

  return Status;
}

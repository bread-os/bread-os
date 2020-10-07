#pragma once
#include "sys.h"
#include "vmem_map.h"

static int vmem_load_uefi() {
  EFI_SYSTEM_TABLE *st = global_env->st;
  gdb_log(L"Checking memory map.\r\n");
  CHECK_STATUS(st->BootServices->GetMemoryMap(
      &vmap_original.size, vmap_original.map, &vmap_original.key,
      &vmap_original.descriptor_size, &vmap_original.descriptor_version));
  for (size_t i = 0; i < 5; i++) {
    if (vmap_original.map != NULL) {
      st->BootServices->FreePool(vmap_original.map);
    }
    if (vmap_original.size > 0) {
      gdb_log(L"Allocating memory\r\n");
      CHECK_STATUS(st->BootServices->AllocatePool(
          EfiLoaderData, vmap_original.size, (void **) &vmap_original.map
      ));
      vmap_original.limit = vmap_original.size;
      EFI_STATUS status = st->BootServices->GetMemoryMap(
          &vmap_original.size, vmap_original.map, &vmap_original.key,
          &vmap_original.descriptor_size, &vmap_original.descriptor_version);
      if (EFI_BUFFER_TOO_SMALL == status) {
        continue;
      } else if (status) {
        CHECK_STATUS(status);
        return -1;
      } else {
        break;
      }
    }
  }
  return 0;
}

#ifdef DEBUG
static void print_map() {
  // todo
}
#else
#define print_map(...)
#endif

static uintptr_t vmem_map_init() {
  if (vmem_load_uefi()) {
    return (uintptr_t) NULL;
  }
  print_map();
}

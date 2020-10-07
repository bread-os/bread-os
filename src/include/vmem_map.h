#pragma once
#include "util.h"

typedef struct vmem_map {
  uintptr_t size;
  uintptr_t limit;
  EFI_MEMORY_DESCRIPTOR *map;
  uintptr_t key;
  uintptr_t descriptor_size;
  uint32_t descriptor_version;
  unsigned virtual: 1;
} vmem_map;

static vmem_map vmap_original;

static int vmem_load_uefi();
static uintptr_t vmem_map_init();

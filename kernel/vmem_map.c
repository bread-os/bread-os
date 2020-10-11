#pragma once
#include "sys.h"
#include "vmem_map.h"

static int vmem_load_uefi() {
  // todo
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

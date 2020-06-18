#pragma once
#include <bprint.h>
#include <bstddef.h>
#include <util.h>

class MemoryManager {
 public:
  static void init_memory_manager();
  static void *malloc(size_t size);
  static bool free(void *ptr);
};

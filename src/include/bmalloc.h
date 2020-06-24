#pragma once
#include <bprint.h>
#include <bstddef.h>
#include <util.h>

namespace bread_os {
class MemoryManager {
 public:
  static auto &instance() {
    static MemoryManager _mm;
    return _mm;
  }
  static void init_memory_manager();
  void *b_malloc(size_t size);

 private:
  MemoryManager() = default;
};

class PhysicalMemoryPage {
 public:
  uint64_t fist_ptr;
  size_t total_size;
  PhysicalMemoryPage *next_page;
};
}  // namespace bread_os

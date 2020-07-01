#pragma once
#include <bprint.h>
#include <bstddef.h>
#include <linked-list.h>
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
  PhysicalMemoryPage() {
    this->fist_ptr = 0;
    this->used_size = 0;
    this->used = false;
  }
  explicit PhysicalMemoryPage(size_t used) {
    this->fist_ptr = 0;
    this->used_size = 0 + used;
    this->used = false;
  };

  // the header of this page
  uint64_t fist_ptr;
  // used size of the page
  size_t used_size;
  bool used;

  // aligned with 4KB
  // see 'EFI_PHYSICAL_ADDRESS EFI_MEMORY_DESCRIPTOR'
  static constexpr const size_t pageSize = 4096;
};

class PhysicalMemoryManager {
 public:
  static auto &instance() {
    static PhysicalMemoryManager _pmm;
    return _pmm;
  }

  typedef LinkedList<PhysicalMemoryPage> PhysicalLinkedList;
  PhysicalLinkedList *freePhysicalMemoryPageList;
  PhysicalLinkedList *usedPhysicalMemoryPageList;
 private:
  PhysicalMemoryManager();
  // deconstruct function should be never called
  ~PhysicalMemoryManager() = default;
};
}  // namespace bread_os

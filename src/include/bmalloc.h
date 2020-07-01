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

  // unsafe
  void *b_malloc(size_t size);

 private:
  MemoryManager() = default;
};

class PhysicalMemoryPage {
 public:
  PhysicalMemoryPage() {
    this->first_ptr = 0;
    this->used_size = 0;
  }
  explicit PhysicalMemoryPage(size_t used) {
    this->first_ptr = 0;
    this->used_size = 0 + used;
  };

  uint64_t current_ptr() { return first_ptr + used_size; }

  // the header of this page
  uint64_t first_ptr;
  // used size of the page
  size_t used_size;

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

  PhysicalMemoryPage *apply_page();
  bool free_page(PhysicalMemoryPage *);

 private:
  PhysicalMemoryManager();
  // deconstruct function should be never called
  ~PhysicalMemoryManager() = default;
  typedef LinkedList PhysicalLinkedList;
  PhysicalLinkedList *usedPhysicalMemoryPageList;
  PhysicalLinkedList *freePhysicalMemoryPageList;
};
}  // namespace bread_os

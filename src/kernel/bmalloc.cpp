#include <bmalloc.h>
namespace bread_os {
static size_t total_memories = 0;
static MMapEnt *current_mmap = nullptr;
static uint64_t current_ptr = 0;

void MemoryManager::init_memory_manager() {
  auto *mmap = &bootboot.mmap;
  current_mmap = mmap;
  current_ptr = MMapEnt_Ptr(mmap);

//  const auto &physicalMemoryManager = PhysicalMemoryManager::instance();
  for (size_t len = 1; len < bootboot.size; len++, mmap++) {
    const size_t current_size = MMapEnt_Size(mmap);

    // other info
    total_memories += current_size; // keep the total memories
  }
}

// only used for kernel
void *MemoryManager::b_malloc(size_t size) {
  const uint64_t total_size = MMapEnt_Size(current_mmap);
  const uint64_t start_ptr = MMapEnt_Ptr(current_mmap);
  uint64_t ptr;
  if (current_ptr + size > start_ptr + total_size) {
    current_mmap++;
    // next mmap
    return b_malloc(size);
  } else {
    ptr = current_ptr;
    current_ptr += size;
  }
  return reinterpret_cast<void *>(ptr);
}

PhysicalMemoryManager::PhysicalMemoryManager() {
  auto &memoryManager = MemoryManager::instance();
  this->freePhysicalMemoryPageList =
      static_cast<PhysicalLinkedList *>(memoryManager.b_malloc(sizeof(PhysicalLinkedList)));
  // at the beginning, usedPhysicalMemoryPageList is empty
  this->usedPhysicalMemoryPageList =
      static_cast<PhysicalLinkedList *>(memoryManager.b_malloc(sizeof(PhysicalLinkedList)));
}

PhysicalMemoryPage *PhysicalMemoryManager::apply_page() {
  auto *page = static_cast<PhysicalMemoryPage *>(MemoryManager::instance().b_malloc(PhysicalMemoryPage::pageSize));
  const size_t pageSize = sizeof(PhysicalMemoryPage);
  const size_t itemSize = sizeof(ListItem);

  page->first_ptr = reinterpret_cast<int64_t>(page) + pageSize;
  auto *item = reinterpret_cast<ListItem *>(page->first_ptr);
  ListItem::clean_up(item);
  item->value = page;
  page->first_ptr += itemSize;
  page->used_size = 0;

  this->usedPhysicalMemoryPageList->append(item);
  return page;
}

bool PhysicalMemoryManager::free_page(PhysicalMemoryPage *physicalMemoryPage) {
  auto *item = reinterpret_cast<ListItem *>(physicalMemoryPage->first_ptr);
  ListItem::clean_up(item);

  physicalMemoryPage->used_size = 0;
  item->value = physicalMemoryPage;
  this->freePhysicalMemoryPageList->append(item);
  return false;
}
}  // namespace bread_os

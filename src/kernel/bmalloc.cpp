#include <bmalloc.h>
namespace bread_os {
static size_t memory_length = 0;

void *kernel::unsafe_malloc(size_t size) {
  // previous few mmap for kernel use
  // will not free for user mode
  auto *mmap = &bootboot.mmap;
  uint64_t ptr = 0;
  do {
    ptr = MMapEnt_Ptr(mmap) + memory_length;
  } while (ptr + size > MMapEnt_Size(mmap), mmap++);
  memory_length += size;
  return reinterpret_cast<void *>(ptr);
}

void MemoryManager::init_memory_manager() {
  auto *mmap = &bootboot.mmap;
  // todo
  for (size_t len = 1; len < bootboot.size; len++, mmap++) {
    const size_t total_size = MMapEnt_Size(mmap);
    const uint64_t fist_ptr = MMapEnt_Ptr(mmap);
  }
}

void *MemoryManager::malloc(size_t size) { return reinterpret_cast<void *>(0); }

bool MemoryManager::register_memory(uint64_t ptr, size_t) {
  // todo
  return false;
}

bool MemoryManager::free(void *ptr) {
  // todo
  return false;
}
}  // namespace bread_os

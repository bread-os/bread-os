#include <bmalloc.h>
namespace bread_os {
static size_t memory_length = 0;

void MemoryManager::init_memory_manager() {
  auto *mmap = &bootboot.mmap;
  // todo
  for (size_t len = 1; len < bootboot.size; len++, mmap++) {
    const size_t total_size = MMapEnt_Size(mmap);
    const uint64_t fist_ptr = MMapEnt_Ptr(mmap);
  }
}
}  // namespace bread_os

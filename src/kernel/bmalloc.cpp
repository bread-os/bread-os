#include <bmalloc.h>
namespace bread_os {
static size_t total_memories = 0;
static MMapEnt *current_mmap = nullptr;
static uint64_t current_ptr = 0;

void MemoryManager::init_memory_manager() {
  auto *mmap = &bootboot.mmap;
  current_mmap = mmap;
  current_ptr = MMapEnt_Ptr(mmap);

  // calculate total memories
  for (size_t len = 1; len < bootboot.size; len++, mmap++) {
    const size_t total_size = MMapEnt_Size(mmap);
    total_memories += total_size;
  }
}

// todo: free()
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
}  // namespace bread_os

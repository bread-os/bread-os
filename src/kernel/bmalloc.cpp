#include <bmalloc.h>

void MemoryManager::init_memory_manager() {
  const auto *mmap = &bootboot.mmap;
  auto total_memories = 0LL;
  for (size_t pos = 0; pos < bootboot.size; pos++) {
    total_memories += MMapEnt_Size(mmap);
  }
  // todo: print all memories;
}

void *MemoryManager::malloc(size_t size) {
  // todo
  return nullptr;
}

bool MemoryManager::free(void *ptr) {
  // todo
  return false;
}

#include <bmalloc.h>
#include <bootboot.h>
#include <bprint.h>
#include <bstddef.h>
#include <util.h>

// entry point
extern "C" void _start() {
  using namespace bread_os;
  auto &printer = Printer::instance();
  printer << "Hello, world!\n";

  MemoryManager::init_memory_manager();

  int x, y, s = bootboot.fb_scanline, w = bootboot.fb_width,
            h = bootboot.fb_height;

  for (y = 0; y < h; y++) {
    *((uint32_t *)(&fb + s * y + (w * 2))) = 0x00FFFFFF;
  }
  for (x = 0; x < w; x++) {
    *((uint32_t *)(&fb + s * (h / 2) + x * 4)) = 0x00FFFFFF;
  }

  while (1)
    ;
}

#include <bprint.h>
#include <bstddef.h>

extern "C" void kmain() {
  const char *hello = "Hello cpp world!";
  volatile auto *vga = (volatile short *) 0xb8000;
  for (size_t pos = 0; hello[pos] != '\0'; ++pos) {
    vga[80 + pos] = ScreenChar(hello[pos]).getContent();
  }
}

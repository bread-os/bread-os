#ifndef BREAD_OS_BOOT
#define BREAD_OS_BOOT
#include <bprint.h>
#include <bstddef.h>

extern "C" void kmain()
{
  const char *hello = "[braed-os:log] kmain load success.";
  Printer &pointer = Printer::instance();
  for (size_t pos = 0; hello[pos] != '\0'; ++pos)
  {
    pointer << ScreenChar(hello[pos]);
  }
}

#endif //BREAD_OS_BOOT

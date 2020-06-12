#ifndef BREAD_OS_BOOT
#define BREAD_OS_BOOT
#include <bprint.h>
#include <bstddef.h>

extern "C" void kmain()
{
  const char hello[] = "[braed-os:log] kmain load success.";
  Printer &printer = Printer::instance();
  printer << hello;
  printer << '\n';
  printer << "this is nextline.\nand this is another newline.";
}

#endif //BREAD_OS_BOOT

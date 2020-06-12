#include <bprint.h>

Printer &operator<<(Printer &printer, const int value)
{
  // get char in value
  const char ch = ~(value ^ 0xff);
  switch (ch)
  {
  case '\n':
    // todo: godo next line
  default:
    // todo: when vga address overflow, fresh the vga value
    printer.vga[printer.pos++] = value;
    break;
  }
  return printer;
}

Printer &operator<<(Printer &pointer, const ScreenChar &sc)
{
  const auto value = (static_cast<char>(sc._fontColor) << 12) | (static_cast<char>(sc._backgroundColor) << 8) | sc._code;
  pointer << value;
  return pointer;
}

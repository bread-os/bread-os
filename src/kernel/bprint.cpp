#include <bstddef.h>
#include <bootboot.h>
#include <bprint.h>
#include <util.h>

Printer &operator<<(Printer &printer, const int value)
{
  // get char in value
  const char ch = ~(value ^ 0xff);
  if (ch == '\n')
  {
    printer.nextline();
    goto end;
  }
  if (printer.pos == 2000)
    printer.upperShift();
  // fixme
end:
  return printer;
}

Printer &operator<<(Printer &printer, const char ch)
{
  printer << ScreenChar(ch);
  return printer;
}

Printer &operator<<(Printer &printer, const char *str)
{
  const auto len = strlen(str);
  for (size_t i = 0; i < len; i++)
    printer << str[i];
  return printer;
}

void Printer::upperShift()
{
  // todo
}

void Printer::nextline()
{
  pos = ((pos / 80) * 80) + 80;
}

Printer &operator<<(Printer &pointer, const ScreenChar &sc)
{
  const auto value = (static_cast<char>(sc._fontColor) << 12) | (static_cast<char>(sc._backgroundColor) << 8) | sc._code;
  pointer << value;
  return pointer;
}

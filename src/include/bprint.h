#pragma once
#include <bstddef.h>
#include <util.h>

class Printer
{
public:
  static Printer &instance()
  {
    static Printer _printer;
    return _printer;
  }

  friend Printer &operator<<(Printer &printer, const int code);
  friend Printer &operator<<(Printer &printer, const char ch);
  friend Printer &operator<<(Printer &printer, const char *str);

private:
  Printer() : pos(0) {}
  inline void upperShift();
  inline void nextline();

  volatile char buf[1024];
  volatile int pos;
};

class ScreenChar
{
public:
  explicit ScreenChar(char ch,
                      Color fontColor = Color::Black,
                      Color backgroundColor = Color::White) : _code(ch), _fontColor(fontColor), _backgroundColor(backgroundColor) {}

  friend Printer &operator<<(Printer &printer, const ScreenChar &sc);

private:
  char _code;
  Color _fontColor;
  Color _backgroundColor;
};

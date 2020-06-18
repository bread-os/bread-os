#pragma once
#include <bstddef.h>
#include <util.h>

class Printer {
 public:
  static Printer &instance() {
    static Printer _printer;
    return _printer;
  }

  friend Printer &operator<<(Printer &printer, const int code);
  friend Printer &operator<<(Printer &printer, const char ch);
  friend Printer &operator<<(Printer &printer, const char *str);

 private:
  Printer() : pos_x(0), pos_y(0) {}
  inline void upperShift();
  inline void nextline();

  volatile int pos_x, pos_y;
};

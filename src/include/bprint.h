#pragma once
#include <bstddef.h>
#include <util.h>

namespace bread_os {
template <typename Arg, typename... Args>
char *format(char *str, Arg &&arg, Args &&... args);
class Printer {
 public:
  static auto &instance() {
    static Printer _printer;
    return _printer;
  }

  friend Printer &operator<<(Printer &, const char);
  friend Printer &operator<<(Printer &, const char *);

 private:
  Printer() : pos_x(0), pos_y(0) {}
  inline void upper_shift();
  inline void nextline();

  volatile int pos_x, pos_y;
};

}  // namespace bread_os

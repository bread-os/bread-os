#pragma once
#include <bstddef.h>
#include <bmalloc.h>
#include <util.h>

namespace bread_os {
template<typename Arg, typename... Args>
char *format(char *str, Args &&... args);

class Printer {
 public:
  static auto &instance() {
    static Printer _printer;
    return _printer;
  }

  friend Printer &operator<<(Printer &, char);
  friend Printer &operator<<(Printer &, const char *);

 private:
  Printer();
  inline void upper_shift();
  inline void next_line();
  char *buffer;
  int pos_x, pos_y;
};

}  // namespace bread_os

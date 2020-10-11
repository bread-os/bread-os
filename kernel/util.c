#include "util.h"

static char to_lowercase(char ch) {
  if (ch >= 'A' && ch <= 'Z')
    return ch - 32;
  else
    return ch;
}

static char to_uppercase(char ch) {
  if (ch >= 'a' && ch <= 'z')
    return ch + 32;
  else
    return ch;
}

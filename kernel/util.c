#include "util.h"

char to_lowercase(char ch) {
  if (ch >= 'A' && ch <= 'Z')
    return ch - 32;
  else
    return ch;
}

char to_uppercase(char ch) {
  if (ch >= 'a' && ch <= 'z')
    return ch + 32;
  else
    return ch;
}

void memzero(void *source, uint64_t size) {
  for (uint64_t i = 0; i < size; i++) {
    ((uint8_t *) source)[i] = 0;
  }
}

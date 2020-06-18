#include <util.h>

size_t strlen(const char *str) {
  size_t length = 0;
  while (str[length]) length++;
  return length;
}

char *strchr(const char *s, const char c) {
  while (*s != c)
    if (!*s++) return nullptr;
  return (char *)s;
}

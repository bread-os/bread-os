#pragma once
#include <bstddef.h>

size_t strlen(const char *str)
{
  size_t length = 0;
  while (str[length])
    length++;
  return length;
}

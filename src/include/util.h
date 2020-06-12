// private header, not public for user side
// not standard implementation
#pragma once
#include <bstddef.h>

size_t strlen(const char *str);

char *strchr(const char *s, const char c);

template <typename T, size_t N>
constexpr size_t arraysize(const T (&)[N])
{
  return N;
}

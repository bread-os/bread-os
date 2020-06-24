// private header, not public for user side
// not standard implementation
#pragma once
typedef unsigned char uint8_t;
typedef short int int16_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef long long unsigned int size_t;
typedef unsigned long long uint64_t;
#include <bootboot.h>
#include <bstddef.h>

extern BOOTBOOT bootboot;
extern unsigned char *environment;
extern uint8_t fb;
extern volatile unsigned char _binary_font_psf_start;
extern volatile unsigned char _binary_font_psf_end;

constexpr const int BUFFER_SIZE = 256;

size_t strlen(const char *str);

char *strchr(const char *s, const char c);

template<typename Type>
void swap(Type &a, Type &b);

template<typename T, typename U>
auto min(T x, U y) -> decltype(x > y ? x : y) {
  return x < y ? x : y;
}

template<typename T, typename U>
auto max(T x, U y) -> decltype(x > y ? x : y) {
  return x > y ? x : y;
}

template<typename T, size_t N>
constexpr size_t arraysize(const T (&)[N]) {
  return N;
}

namespace bread {

template<class T>
struct remove_reference { typedef T type; };

template<class T>
struct remove_reference<T &> { typedef T type; };

template<class T>
struct remove_reference<T &&> { typedef T type; };

template<class T>
inline T &&forward(typename remove_reference<T>::type &t) noexcept {
  return static_cast<T &&>(t);
}
}

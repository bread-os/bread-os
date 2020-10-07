#pragma once
#ifndef TYPES_H
#define TYPES_H

/* *****************************************************************************
Core Types
***************************************************************************** */
typedef unsigned char uint8_t;
typedef char int8_t;
/*
typedef uint8_t _Bool;
*/
_Static_assert(sizeof(int8_t) == 1, "int8_t != 8 bits");
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
#ifdef __cplusplus
typedef uint8_t _Bool;
#else
typedef uint16_t char16_t;
typedef unsigned int char32_t;
typedef char16_t wchar_t;
#endif
_Static_assert(sizeof(char16_t) == 2, "char16_t != 16 bits");
_Static_assert(sizeof(uint32_t) == 4, "int32_t != 32 bits");
typedef unsigned long long uint64_t;
typedef long long int64_t;
_Static_assert(sizeof(int64_t) == 8, "int64_t != 64 bits");
_Static_assert(sizeof(void *) == 8, "void * != 64 bits");
typedef unsigned long long uintptr_t;
typedef long long intptr_t;
_Static_assert(sizeof(void *) == 8, "intptr_t != 64 bits");
typedef uintptr_t size_t;
typedef intptr_t ssize_t;

/* *****************************************************************************
Type extensions and naming alternatives
***************************************************************************** */

/* clang extensions for 128bit*/
#if defined(__has_feature)
#if __has_feature(__int128_t) == 1

typedef __int128_t int128_t;
typedef __uint128_t uint128_t;
typedef int128_t max_align_t;

#else
typedef int64_t max_align_t;
#endif

#else
typedef int64_t max_align_t;
#endif
/* fast signed */
typedef int8_t int_fast8_t;
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int64_t int_fast64_t;
/* signed minimal */
typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;
/* */
typedef ssize_t intmax_t;

typedef uint8_t uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;

typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

typedef size_t uintmax_t;
typedef intptr_t ptrdiff_t;

/* *****************************************************************************
Macros
***************************************************************************** */

#ifndef NULL
#define NULL ((void *)0x00ULL)
#endif

#define alignas(algn) _Alignas(algn)
#define alignof(algn) _Alignof(algn)
#define __alignas_is_defined 1
#define __alignof_is_defined 1
#define noreturn _Noreturn

#define INT8_MIN ((int8_t)((uint8_t)1 << 7))
#define INT16_MIN ((int16_t)((uint16_t)1 << 15))
#define INT32_MIN ((int32_t)((uint32_t)1UL << 31))
#define INT64_MIN ((int64_t)((uint64_t)1ULL << 63))

#define INT8_MAX ((int8_t)((~((uint8_t)0)) >> 1))
#define INT16_MAX ((int16_t)((~((uint16_t)0)) >> 1))
#define INT32_MAX ((int32_t)((~((uint32_t)0L)) >> 1))
#define INT64_MAX ((int64_t)((~((uint64_t)0LL)) >> 1))

#define UINT8_MAX (~((uint8_t)0))
#define UINT16_MAX (~((uint16_t)0))
#define UINT32_MAX (~((uint32_t)0UL))
#define UINT64_MAX (~((uint64_t)0ULL))

#define INT64_C(i) ((int64_t)(i##LL))
#define INTMAX_C(i) ((intmax_t)(i)##LL)
#define UINT64_C(i) ((uint64_t)(i##ULL))
#define UINTMAX_C(i) ((uintmax_t)(i##ULL))

#define INT8_C(i) ((int8_t)(i))
#define INT16_C(i) ((int16_t)(i))
#define INT32_C(i) ((int32_t)(i))

#define UINT8_C(i) ((uint8_t)(i))
#define UINT16_C(i) ((uint16_t)(i))
#define UINT32_C(i) ((uint32_t)(i))

#define INT_FAST8_MIN INT8_MIN
#define INT_FAST16_MIN INT16_MIN
#define INT_FAST32_MIN INT32_MIN
#define INT_FAST64_MIN INT64_MIN

#define INT_LEAST8_MIN INT8_MIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN INT64_MIN

#define INTPTR_MIN INT64_MIN
#define INTMAX_MIN INT64_MIN

#define INT_FAST8_MAX INT8_MAX
#define INT_FAST16_MAX INT16_MAX
#define INT_FAST32_MAX INT32_MAX
#define INT_FAST64_MAX INT64_MAX

#define INT_LEAST8_MAX INT8_MAX
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MAX INT64_MAX

#define INTPTR_MAX INT64_MAX
#define INTMAX_MAX INT64_MAX

#define UINT_FAST8_MAX UINT8_MAX
#define UINT_FAST16_MAX UINT16_MAX
#define UINT_FAST32_MAX UINT32_MAX
#define UINT_FAST64_MAX UINT64_MAX

#define UINT_LEAST8_MAX UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#define UINTPTR_MAX UINT64_MAX
#define UINTMAX_MAX UINT64_MAX

#endif

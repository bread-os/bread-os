#pragma once

typedef unsigned char uint8_t;
typedef short int int16_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef long long unsigned int size_t;
typedef unsigned long long uint64_t;

// refs: https://wiki.osdev.org/PC_Screen_Font
typedef struct {
  uint32_t magic;
  uint32_t version;
  uint32_t headersize;
  uint32_t flags;
  uint32_t numglyph;
  uint32_t bytesperglyph;
  uint32_t height;
  uint32_t width;
  uint8_t glyphs;
} PSF_font;

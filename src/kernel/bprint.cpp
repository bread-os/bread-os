#include <bprint.h>
namespace bread_os {
// refs: https://wiki.osdev.org/PC_Screen_Font
void putchar(unsigned short int c, uint32_t cx, uint32_t cy,
    // todo: color
             uint32_t fg = 0xFFFFFF, uint32_t bg = 0) {
  PSF_font *font = (PSF_font *) &_binary_font_psf_start;
  uint32_t bytesperline = (font->width + 7) / 8;
  unsigned char *glyph =
      (unsigned char *) &_binary_font_psf_start + font->headersize +
          (c > 0 && c < font->numglyph ? c : 0) * font->bytesperglyph;
  uint32_t offs =
      (cy * font->height * bootboot.fb_scanline) + (cx * (font->width + 1) * 4);
  size_t x, y, line, mask;
  for (y = 0; y < font->height; y++) {
    line = offs;
    mask = (1U << (font->width - 1));
    for (x = 0; x < font->width; x++) {
      *((uint32_t *) (&fb + line)) = ((uint32_t) *glyph) & (mask) ? fg : bg;
      mask >>= 1U;
      line += 4;
    }
    glyph += bytesperline;
    offs += bootboot.fb_scanline;
  }
}

template<typename Num>
size_t integer_to_chars(Num n, char *str) {
  size_t pos = 0;
  for (; n; n /= 10, pos++)
    str[pos] = '0' + (n % 10);
  for (size_t tmp = 0; str + tmp != str + pos - tmp; tmp++)
    swap(str[tmp], str[pos - tmp]);
  return pos;
}

// tip: unsafe
template<typename Arg, typename... Args>
char *format(char *str, Arg &&arg, Args &&... args) {
  // find first '%'
  size_t i = 0;
  const auto *pos = strchr(str, '%');
  while (pos != nullptr) {
    const char ch = *(pos + 1);
    if (ch == 'd') {
      const size_t len = integer_to_chars(arg, str + i);
      i += len;
    } else {
      // todo
    }
    // find next '%'
    pos = strchr(str + (pos - str + 1), '%');
  }
  // todo
  return nullptr;
}

Printer &operator<<(Printer &printer, const char ch) {
  if (ch == '\n') {
    printer.next_line();
    goto end;
  } else if (printer.pos_x == 90) {
    printer.next_line();
  }
  // todo: go back the top when printer overflow
  // fixme: color support
  putchar(ch, printer.pos_x++, printer.pos_y /*, fg, bg */);
  end:
  return printer;
}

Printer &operator<<(Printer &printer, const char *str) {
  const auto len = strlen(str);
  for (size_t i = 0; i < len; i++) printer << str[i];
  return printer;
}

void Printer::upper_shift() {
  // todo
}

void Printer::next_line() {
  // 90 * 37
  pos_x = 0;
  pos_y++;
  if (pos_y == 37) {
    pos_y = 0;
  }
}

Printer::Printer() : pos_x(0), pos_y(0) {
  buffer = static_cast<char *>(kernel::unsafe_malloc(BUFFER_SIZE));
}

}  // namespace bread_os

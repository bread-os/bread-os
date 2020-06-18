#include <bprint.h>
namespace bread_os {
// refs: https://wiki.osdev.org/PC_Screen_Font
void putchar(unsigned short int c, int cx, int cy,
             // todo: color
             uint32_t fg = 0xFFFFFF, uint32_t bg = 0) {
  PSF_font *font = (PSF_font *)&_binary_font_psf_start;
  int bytesperline = (font->width + 7) / 8;
  unsigned char *glyph =
      (unsigned char *)&_binary_font_psf_start + font->headersize +
      (c > 0 && c < font->numglyph ? c : 0) * font->bytesperglyph;
  int offs =
      (cy * font->height * bootboot.fb_scanline) + (cx * (font->width + 1) * 4);
  size_t x, y, line, mask;
  for (y = 0; y < font->height; y++) {
    line = offs;
    mask = 1 << (font->width - 1);
    for (x = 0; x < font->width; x++) {
      *((uint32_t *)(&fb + line)) = ((int)*glyph) & (mask) ? fg : bg;
      mask >>= 1;
      line += 4;
    }
    glyph += bytesperline;
    offs += bootboot.fb_scanline;
  }
}

Printer &operator<<(Printer &printer, const char ch) {
  if (ch == '\n') {
    printer.nextline();
    goto end;
  } else if (printer.pos_x == 90) {
    printer.nextline();
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

void Printer::nextline() {
  // 90 * 37
  pos_x = 0;
  pos_y++;
  if (pos_y == 37) {
    pos_y = 0;
  }
}

}  // namespace bread_os

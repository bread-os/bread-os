#include <bstddef.h>
#include <bootboot.h>
#include <bprint.h>

extern BOOTBOOT bootboot;
extern unsigned char *environment;
extern uint8_t fb;

// entry point
void _start()
{
  int x, y, s = bootboot.fb_scanline, w = bootboot.fb_width, h = bootboot.fb_height;

  for (y = 0; y < h; y++)
  {
    *((uint32_t *)(&fb + s * y + (w * 2))) = 0x00FFFFFF;
  }
  for (x = 0; x < w; x++)
  {
    *((uint32_t *)(&fb + s * (h / 2) + x * 4)) = 0x00FFFFFF;
  }

  while (1)
    ;
}

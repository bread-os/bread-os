/* we don't assume stdint.h exists */
typedef short int int16_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;

#include <bootboot.h>

/* imported virtual addresses, see linker script */
extern BOOTBOOT bootboot;          // see bootboot.h
extern unsigned char *environment; // configuration, UTF-8 text key=value pairs
extern uint8_t fb;                 // linear framebuffer mapped

/******************************************
 * Entry point, called by BOOTBOOT Loader *
 ******************************************/
void _start()
{
  /*** NOTE: this code runs on all cores in parallel ***/
  int x, y, s = bootboot.fb_scanline, w = bootboot.fb_width, h = bootboot.fb_height;

  // cross-hair to see screen dimension detected correctly
  for (y = 0; y < h; y++)
  {
    *((uint32_t *)(&fb + s * y + (w * 2))) = 0x00FFFFFF;
  }
  for (x = 0; x < w; x++)
  {
    *((uint32_t *)(&fb + s * (h / 2) + x * 4)) = 0x00FFFFFF;
  }

  // hang for now
  while (1)
    ;
}

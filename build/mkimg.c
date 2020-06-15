/*
 * images/mkimg.c
 *
 * Copyright (C) 2017 - 2020 bzt (bztsrc@gitlab)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This file is part of the BOOTBOOT Protocol package.
 * @brief Small tool to create disk or cdrom images with BOOTBOOT
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

long int read_size;

/**
 * Read file into memory
 */
unsigned char *readfileall(char *file)
{
  unsigned char *data = NULL;
  FILE *f;
  read_size = 0;
  f = fopen(file, "r");
  if (f)
  {
    fseek(f, 0L, SEEK_END);
    read_size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    data = (unsigned char *)malloc(read_size + 1);
    if (data == NULL)
    {
      fprintf(stderr, "mkimg: Unable to allocate %ld memory\n", read_size + 1);
      exit(1);
    }
    memset(data, 0, read_size + 1);
    fread(data, read_size, 1, f);
    fclose(f);
  }
  return data;
}

/**
 * Set integers in byte arrays
 */
int getint(unsigned char *ptr) { return (unsigned char)ptr[0] + (unsigned char)ptr[1] * 256 + (unsigned char)ptr[2] * 256 * 256 + ptr[3] * 256 * 256 * 256; }
void setint(int val, unsigned char *ptr) { memcpy(ptr, &val, 4); }
void setinte(int val, unsigned char *ptr)
{
  char *v = (char *)&val;
  memcpy(ptr, &val, 4);
  ptr[4] = v[3];
  ptr[5] = v[2];
  ptr[6] = v[1];
  ptr[7] = v[0];
}

/**
 * CRC-stuff
 */
unsigned int crc32_lookup[256] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3, 0x0edb8832,
    0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 0x136c9856, 0x646ba8c0, 0xfd62f97a,
    0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3,
    0x45df5c75, 0xdcd60dcf, 0xabd13d59, 0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab,
    0xb6662d3d, 0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01, 0x6b6b51f4,
    0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65, 0x4db26158, 0x3ab551ce, 0xa3bc0074,
    0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525,
    0x206f85b3, 0xb966d409, 0xce61e49f, 0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615,
    0x73dc1683, 0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7, 0xfed41b76,
    0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b, 0xd80d2bda, 0xaf0a1b4c, 0x36034af6,
    0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7,
    0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d, 0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7,
    0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45, 0xa00ae278,
    0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9, 0xbdbdf21c, 0xcabac28a, 0x53b39330,
    0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d};

unsigned int crc32a_calc(char *start, int length)
{
  unsigned int crc32_val = 0xffffffff;
  while (length--)
    crc32_val = (crc32_val >> 8) ^ crc32_lookup[(crc32_val & 0xff) ^ (unsigned char)*start++];
  crc32_val ^= 0xffffffff;
  return crc32_val;
}

/**
 * Create a ROM image of the initrd
 */
void initrdrom()
{
  int i, size;
  unsigned char *fs, *buf, c = 0;
  FILE *f;

  fs = readfileall("initrd.bin");
  size = ((read_size + 32 + 511) / 512) * 512;
  if (fs == NULL)
  {
    fprintf(stderr, "mkimg: unable to load initrd.bin\n");
    exit(2);
  }
  buf = (unsigned char *)malloc(size + 1);
  /* Option ROM header */
  buf[0] = 0x55;
  buf[1] = 0xAA;
  buf[2] = (read_size + 32 + 511) / 512;
  /* asm "xor ax,ax; retf" */
  buf[3] = 0x31;
  buf[4] = 0xC0;
  buf[5] = 0xCB;
  /* identifier, size and data */
  memcpy(buf + 8, "INITRD", 6);
  memcpy(buf + 16, &read_size, 4);
  memcpy(buf + 32, fs, read_size);
  /* checksum */
  for (i = 0; i < size; i++)
    c += buf[i];
  buf[6] = (unsigned char)((int)(256 - c));
  /* write out */
  f = fopen("initrd.rom", "wb");
  if (!f)
  {
    fprintf(stderr, "mkimg: unable to write initrd.rom\n");
    exit(3);
  }
  fwrite(buf, size, 1, f);
  fclose(f);
}

/**
 * Create a hybrid disk image from partition image with initrd in it
 */
int createdisk(int disksize, char *diskname)
{
  unsigned long int i, j = 0, gs = 63 * 512, es, bbs = 0;
  unsigned long int uuid[4] = {0x12345678, 0x12345678, 0x12345678, 0x12345678};
  unsigned char *esp, *gpt, *iso, *p, *loader;
  time_t t = time(NULL);
  struct tm *ts = gmtime(&t);
  char isodate[17];
  FILE *f;

  esp = readfileall("./bootpart.bin");
  es = read_size;
  gpt = malloc(gs + 512);
  memset(gpt, 0, gs + 512);
  iso = malloc(32768);
  memset(iso, 0, 32768);
  if (disksize < 64 * 1024 * 1024)
    disksize = 64 * 1024 * 1024;
  /* make the UUID unique */
  uuid[0] ^= (unsigned long int)t;

  /* MBR / VBR stage 1 loader */
  loader = readfileall("./boot.bin");
  if (loader == NULL)
  {
    loader = malloc(512);
    memset(loader, 0, 512);
  }
  else
  {
    memset(loader + 0x1B8, 0, 0x1FE - 0x1B8);
  }
  /* search for stage2 loader (FS0:\BOOTBOOT\LOADER) */
  if (es > 0)
  {
    for (i = 0; i < es - 512; i += 512)
    {
      if ((unsigned char)esp[i + 0] == 0x55 &&
          (unsigned char)esp[i + 1] == 0xAA &&
          (unsigned char)esp[i + 3] == 0xE9 &&
          (unsigned char)esp[i + 8] == 'B' &&
          (unsigned char)esp[i + 12] == 'B')
      {
        bbs = ((i + 65536) / 512);
        break;
      }
    }
  }
  /* save stage2 address into stage1 */
  if (bbs > 0)
  {
    setint(bbs, loader + 0x1B0);
    /* WinNT disk id */
    setint(uuid[0], loader + 0x1B8);
  }
  else
  {
    fprintf(stderr, "mkimg: FS0:\\BOOTBOOT\\LOADER not found\n");
    memset(loader, 0, 512);
  }
  /* magic */
  loader[0x1FE] = 0x55;
  loader[0x1FF] = 0xAA;

  /* copy stage1 loader into VBR too */
  if (loader[0] != 0 && es > 0)
  {
    /* skip BPB */
    memcpy(esp, loader, 11);
    memcpy(esp + 0x5A, loader + 0x5A, 512 - 0x5A);
  }

  /* generate PMBR partitioning table */
  j = 0x1C0;
  if (es > 0)
  {
    /* MBR, EFI System Partition */
    loader[j - 2] = 0x80;                          /* bootable flag */
    setint(129, loader + j);                       /* start CHS */
    loader[j + 2] = esp[0x39] == '1' ? 0xE : 0xC;  /* type, LBA FAT16 (0xE) or FAT32 (0xC) */
    setint(((gs + es) / 512) + 2, loader + j + 4); /* end CHS */
    setint(128, loader + j + 6);                   /* start LBA */
    setint(((es) / 512), loader + j + 10);         /* number of sectors */
    j += 16;
  }
  /* MBR, GPT entry */
  setint(1, loader + j);                  /* start CHS */
  loader[j + 2] = 0xEE;                   /* type */
  setint((gs / 512) + 1, loader + j + 4); /* end CHS */
  setint(1, loader + j + 6);              /* start LBA */
  setint((gs / 512), loader + j + 10);    /* number of sectors */
  j += 16;

  /* GPT header */
  memset(gpt, 0, gs);
  memcpy(gpt, "EFI PART", 8);             /* magic */
  setint(1, gpt + 10);                    /* revision */
  setint(92, gpt + 12);                   /* size */
  setint(1, gpt + 24);                    /* primary LBA */
  setint(disksize / 512 - 1, gpt + 32);   /* secondary LBA */
  setint((gs / 512) + 1, gpt + 40);       /* first usable LBA */
  setint((disksize / 512) - 1, gpt + 48); /* last usable LBA */
  setint(uuid[0], gpt + 56);              /* disk UUID */
  setint(uuid[1], gpt + 60);
  setint(uuid[2], gpt + 64);
  setint(uuid[3], gpt + 68);
  setint(2, gpt + 72);          /* partitioning table LBA */
  setint(es ? 1 : 0, gpt + 80); /* number of entries */
  setint(128, gpt + 84);        /* size of one entry */

  p = gpt + 512;
  /* GPT, EFI System Partition (ESP, /boot) */
  if (es > 0)
  {
    setint(0x0C12A7328, p); /* entry type */
    setint(0x011D2F81F, p + 4);
    setint(0x0A0004BBA, p + 8);
    setint(0x03BC93EC9, p + 12);
    setint(uuid[0] + 1, p + 16); /* partition UUID */
    setint(uuid[1], p + 20);
    setint(uuid[2], p + 24);
    setint(uuid[3], p + 28);
    setint(128, p + 32);                         /* start LBA */
    setint(((es) / 512) + 127, p + 40);          /* end LBA */
    memcpy(p + 64, L"EFI System Partition", 42); /* name */
    p += 128;
  }

  /* calculate checksums */
  /* partitioning table */
  i = (int)(gpt[80] * gpt[84]);
  setint(crc32a_calc((char *)gpt + 512, i), gpt + 88);
  /* header */
  i = getint(gpt + 12); /* size of header */
  setint(0, gpt + 16);  /* calculate as zero */
  setint(crc32a_calc((char *)gpt, i), gpt + 16);

  /* ISO9660 cdrom image part */
  if (bbs % 4 != 0)
  {
    fprintf(stderr, "mkimg: %s\n", "Stage2 is not 2048 byte sector aligned");
    exit(3);
  }
  sprintf((char *)&isodate, "%04d%02d%02d%02d%02d%02d00",
          ts->tm_year + 1900, ts->tm_mon + 1, ts->tm_mday, ts->tm_hour, ts->tm_min, ts->tm_sec);
  /* 16th sector: Primary Volume Descriptor */
  iso[0] = 1; /* Header ID */
  memcpy(&iso[1], "CD001", 5);
  iso[6] = 1; /* version */
  for (i = 8; i < 72; i++)
    iso[i] = ' ';
  memcpy(&iso[40], "BOOTBOOT_CD", 11); /* Volume Identifier */
  setinte((65536 + es + 2047) / 2048, &iso[80]);
  iso[120] = iso[123] = 1;  /* Volume Set Size */
  iso[124] = iso[127] = 1;  /* Volume Sequence Number */
  iso[129] = iso[130] = 8;  /* logical blocksize (0x800) */
  iso[156] = 0x22;          /* root directory recordsize */
  setinte(20, &iso[158]);   /* root directory LBA */
  setinte(2048, &iso[166]); /* root directory size */
  iso[174] = ts->tm_year;   /* root directory create date */
  iso[175] = ts->tm_mon + 1;
  iso[176] = ts->tm_mday;
  iso[177] = ts->tm_hour;
  iso[178] = ts->tm_min;
  iso[179] = ts->tm_sec;
  iso[180] = 0; /* timezone UTC (GMT) */
  iso[181] = 2; /* root directory flags (0=hidden,1=directory) */
  iso[184] = 1; /* root directory number */
  iso[188] = 1; /* root directory filename length */
  for (i = 190; i < 813; i++)
    iso[i] = ' '; /* Volume data */
  memcpy(&iso[318], "BOOTBOOT <HTTPS://GITLAB.COM/BZTSRC/BOOTBOOT>", 45);
  memcpy(&iso[446], "BOOTBOOT MKIMG", 14);
  memcpy(&iso[574], "BOOTBOOT CD", 11);
  for (i = 702; i < 813; i++)
    iso[i] = ' ';                  /* file descriptors */
  memcpy(&iso[813], &isodate, 16); /* volume create date */
  memcpy(&iso[830], &isodate, 16); /* volume modify date */
  for (i = 847; i < 863; i++)
    iso[i] = '0'; /* volume expiration date */
  for (i = 864; i < 880; i++)
    iso[i] = '0'; /* volume shown date */
  iso[881] = 1;   /* filestructure version */
  for (i = 883; i < 1395; i++)
    iso[i] = ' '; /* file descriptors */
  /* 17th sector: Boot Record Descriptor */
  iso[2048] = 0; /* Header ID */
  memcpy(&iso[2049], "CD001", 5);
  iso[2054] = 1; /* version */
  memcpy(&iso[2055], "EL TORITO SPECIFICATION", 23);
  setinte(19, &iso[2048 + 71]); /* Boot Catalog LBA */
  /* 18th sector: Volume Descritor Terminator */
  iso[4096] = 0xFF; /* Header ID */
  memcpy(&iso[4097], "CD001", 5);
  iso[4102] = 1; /* version */
  /* 19th sector: Boot Catalog */
  /* --- BIOS, Validation Entry + Initial/Default Entry --- */
  iso[6144] = 1;    /* Header ID, Validation Entry */
  iso[6145] = 0;    /* Platform 80x86 */
  iso[6172] = 0xaa; /* magic bytes */
  iso[6173] = 0x55;
  iso[6174] = 0x55;
  iso[6175] = 0xaa;
  iso[6176] = 0x88;            /* Bootable, Initial/Default Entry */
  iso[6182] = 4;               /* Sector Count */
  setint(128 / 4, &iso[6184]); /* Boot Record LBA */
  /* --- UEFI, Final Section Header Entry + Section Entry --- */
  iso[6208] = 0x91;            /* Header ID, Final Section Header Entry */
  iso[6209] = 0xEF;            /* Platform EFI */
  iso[6210] = 1;               /* Number of entries */
  iso[6240] = 0x88;            /* Bootable, Section Entry */
  setint(128 / 4, &iso[6248]); /* ESP Start LBA */
  /* 20th sector: Root Directory */
  /* . */
  iso[8192] = 0x22;          /* recordsize */
  setinte(20, &iso[8194]);   /* LBA */
  setinte(2048, &iso[8202]); /* size */
  iso[8210] = ts->tm_year;   /* date */
  iso[8211] = ts->tm_mon + 1;
  iso[8212] = ts->tm_mday;
  iso[8213] = ts->tm_hour;
  iso[8214] = ts->tm_min;
  iso[8215] = ts->tm_sec;
  iso[8216] = 0; /* timezone UTC (GMT) */
  iso[8217] = 2; /* flags (0=hidden,1=directory) */
  iso[8220] = 1; /* serial */
  iso[8224] = 1; /* filename length */
  /* .. */
  iso[8226] = 0x22;          /* recordsize */
  setinte(20, &iso[8228]);   /* LBA */
  setinte(2048, &iso[8236]); /* size */
  iso[8244] = ts->tm_year;   /* date */
  iso[8245] = ts->tm_mon + 1;
  iso[8246] = ts->tm_mday;
  iso[8247] = ts->tm_hour;
  iso[8248] = ts->tm_min;
  iso[8249] = ts->tm_sec;
  iso[8250] = 0; /* timezone UTC (GMT) */
  iso[8251] = 2; /* flags (0=hidden,1=directory) */
  iso[8254] = 1; /* serial */
  iso[8258] = 2; /* filename length */
  /* README.TXT */
  iso[8260] = 0x22 + 12;    /* recordsize */
  setinte(21, &iso[8262]);  /* LBA */
  setinte(130, &iso[8270]); /* size */
  iso[8278] = ts->tm_year;  /* date */
  iso[8279] = ts->tm_mon + 1;
  iso[8280] = ts->tm_mday;
  iso[8281] = ts->tm_hour;
  iso[8282] = ts->tm_min;
  iso[8283] = ts->tm_sec;
  iso[8284] = 0;  /* timezone UTC (GMT) */
  iso[8285] = 0;  /* flags (0=hidden,1=directory) */
  iso[8288] = 1;  /* serial */
  iso[8292] = 12; /* filename length */
  memcpy(&iso[8293], "README.TXT;1", 12);
  /* 21th sector: contents of README.TXT */
  memcpy(&iso[10240], "BOOTBOOT Live Image\r\n\r\nBootable as\r\n"
                      " - CDROM (El Torito, UEFI)\r\n"
                      " - USB stick (BIOS, Multiboot, UEFI)\r\n"
                      " - SD card (Raspberry Pi 3+)",
         130);

  f = fopen(diskname, "wb");
  if (!f)
  {
    fprintf(stderr, "mkimg: unable to write %s\n", diskname);
    exit(2);
  }
  /* (P)MBR */
  fwrite(loader, 512, 1, f);
  /* GPT header + entries */
  fwrite(gpt, gs, 1, f);
  /* ISO9660 descriptors */
  fwrite(iso, 32768, 1, f);
  /* Partitions */
  if (es > 0)
    fwrite(esp, es, 1, f);
  fseek(f, disksize - gs, SEEK_SET);

  /* GPT entries again */
  fwrite(gpt + 512, gs - 512, 1, f);
  /* GPT secondary header */
  i = getint(gpt + 32);
  setint(getint(gpt + 24), gpt + 32); /* secondary lba */
  setint(i, gpt + 24);                /* primary lba */

  setint((i * 512 - gs) / 512 + 1, gpt + 72); /* partition lba */
  i = getint(gpt + 12);                       /* size of header */
  setint(0, gpt + 16);                        /* calculate with zero */
  setint(crc32a_calc((char *)gpt, i), gpt + 16);
  fwrite(gpt, 512, 1, f);
  fclose(f);
  return 1;
}

/**
 * Main entry point
 */
int main(int argc, char **argv)
{
  if (argc < 2 || argv[1] == NULL || !strcmp(argv[1], "help") || (strcmp(argv[1], "rom") && argc < 3))
  {
    printf("BOOTBOOT mkimg utility - bztsrc@gitlab\n\nUsage:\n"
           "  ./mkimg <disk image size in megabytes> <disk image name>\n"
           "  ./mkimg rom\n\n"
           "Creates a hybrid disk / cdrom image from bootpart.bin, or initrd.rom from initrd.bin.\n");
    exit(0);
  }
  if (!strcmp(argv[1], "rom"))
    initrdrom();
  else
    createdisk(atoi(argv[1]) * 1024 * 1024, argv[2]);
  return 0;
}

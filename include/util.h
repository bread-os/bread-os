#pragma once
#include <efi.h>
#include <efilib.h>
#include <stdint-gcc.h>
#define UEFI_MMAP_SIZE 0x4000
#define PAGE_SIZE 4096

static char to_lowercase(char ch);
static char to_uppercase(char ch);

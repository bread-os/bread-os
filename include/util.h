#pragma once
#include <efi.h>
#include <efilib.h>
#include <stdint-gcc.h>
#define UEFI_MMAP_SIZE 0x4000
#define PAGE_SIZE 4096

char to_lowercase(char ch);
char to_uppercase(char ch);
void memzero(void *source, uint64_t size);

#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include "sys.h"

const static CHAR16 *os_name = L"bread-os";
// 536237 in decimal
const static int64_t magic_number = 0x82EAD;

void print_elf_header64(Elf64_Ehdr elf_header) {
  /* Storage capacity class */
  Print(L"Storage class\t= ");
  switch (elf_header.e_ident[EI_CLASS]) {
    case ELFCLASS32:Print(L"32-bit objects\r\n");
      break;

    case ELFCLASS64:Print(L"64-bit objects\r\n");
      break;

    default:Print(L"INVALID CLASS\r\n");
      break;
  }

  /* Data Format */
  Print(L"Data format\t= ");
  switch (elf_header.e_ident[EI_DATA]) {
    case ELFDATA2LSB:Print(L"2's complement, little endian\r\n");
      break;

    case ELFDATA2MSB:Print(L"2's complement, big endian\r\n");
      break;

    default:Print(L"INVALID Format\r\n");
      break;
  }

  /* OS ABI */
  Print(L"OS ABI\t\t= ");
  switch (elf_header.e_ident[EI_OSABI]) {
    case ELFOSABI_SYSV:Print(L"UNIX System V ABI\r\n");
      break;

    case ELFOSABI_HPUX:Print(L"HP-UX\r\n");
      break;

    case ELFOSABI_NETBSD:Print(L"NetBSD\r\n");
      break;

    case ELFOSABI_LINUX:Print(L"Linux\r\n");
      break;

    case ELFOSABI_SOLARIS:Print(L"Sun Solaris\r\n");
      break;

    case ELFOSABI_AIX:Print(L"IBM AIX\r\n");
      break;

    case ELFOSABI_IRIX:Print(L"SGI Irix\r\n");
      break;

    case ELFOSABI_FREEBSD:Print(L"FreeBSD\r\n");
      break;

    case ELFOSABI_TRU64:Print(L"Compaq TRU64 UNIX\r\n");
      break;

    case ELFOSABI_MODESTO:Print(L"Novell Modesto\r\n");
      break;

    case ELFOSABI_OPENBSD:Print(L"OpenBSD\r\n");
      break;

    case ELFOSABI_ARM_AEABI:Print(L"ARM EABI\r\n");
      break;

    case ELFOSABI_ARM:Print(L"ARM\r\n");
      break;

    case ELFOSABI_STANDALONE:Print(L"Standalone (embedded) app\r\n");
      break;

    default:Print(L"Unknown (0x%x)\r\n", elf_header.e_ident[EI_OSABI]);
      break;
  }

  /* ELF filetype */
  Print(L"Filetype \t= ");
  switch (elf_header.e_type) {
    case ET_NONE:Print(L"N/A (0x0)\r\n");
      break;

    case ET_REL:Print(L"Relocatable\r\n");
      break;

    case ET_EXEC:Print(L"Executable\r\n");
      break;

    case ET_DYN:Print(L"Shared Object\r\n");
      break;
    default:Print(L"Unknown (0x%x)\r\n", elf_header.e_type);
      break;
  }

  /* ELF Machine-id */
  Print(L"Machine\t\t= ");
  switch (elf_header.e_machine) {
    case EM_NONE:Print(L"None (0x0)\r\n");
      break;

    case EM_386:Print(L"INTEL x86 (0x%x)\r\n", EM_386);
      break;

    case EM_X86_64:Print(L"AMD x86_64 (0x%x)\r\n", EM_X86_64);
      break;

    case EM_AARCH64:Print(L"AARCH64 (0x%x)\r\n", EM_AARCH64);
      break;

    default:Print(L" 0x%x\r\n", elf_header.e_machine);
      break;
  }

  /* Entry point */
  Print(L"Entry point\t= 0x%08lx\r\n", elf_header.e_entry);

  /* ELF header size in bytes */
  Print(L"ELF header size\t= 0x%08x\r\n", elf_header.e_ehsize);

  /* Program Header */
  Print(L"\r\nProgram Header\t= ");
  Print(L"0x%08lx\r\n", elf_header.e_phoff);        /* start */
  Print(L"\t\t  %d entries\r\n", elf_header.e_phnum);    /* num entry */
  Print(L"\t\t  %d bytes\r\n", elf_header.e_phentsize);    /* size/entry */

  /* Section header starts at */
  Print(L"\r\nSection Header\t= ");
  Print(L"0x%08lx\r\n", elf_header.e_shoff);        /* start */
  Print(L"\t\t  %d entries\r\n", elf_header.e_shnum);    /* num entry */
  Print(L"\t\t  %d bytes\r\n", elf_header.e_shentsize);    /* size/entry */
  Print(L"\t\t  0x%08x (string table offset)\r\n", elf_header.e_shstrndx);

  /* File flags (Machine specific)*/
  Print(L"\r\nFile flags \t= 0x%08x\r\n", elf_header.e_flags);

  /* ELF file flags are machine specific.
   * INTEL implements NO flags.
   * ARM implements a few.
   * Add support below to parse ELF file flags on ARM
   */
  int32_t ef = elf_header.e_flags;
  Print(L"\t\t  ");

  if (ef & EF_ARM_RELEXEC)
    Print(L",RELEXEC ");

  if (ef & EF_ARM_HASENTRY)
    Print(L",HASENTRY ");

  if (ef & EF_ARM_INTERWORK)
    Print(L",INTERWORK ");

  if (ef & EF_ARM_APCS_26)
    Print(L",APCS_26 ");

  if (ef & EF_ARM_APCS_FLOAT)
    Print(L",APCS_FLOAT ");

  if (ef & EF_ARM_PIC)
    Print(L",PIC ");

  if (ef & EF_ARM_ALIGN8)
    Print(L",ALIGN8 ");

  if (ef & EF_ARM_NEW_ABI)
    Print(L",NEW_ABI ");

  if (ef & EF_ARM_OLD_ABI)
    Print(L",OLD_ABI ");

  if (ef & EF_ARM_SOFT_FLOAT)
    Print(L",SOFT_FLOAT ");

  if (ef & EF_ARM_VFP_FLOAT)
    Print(L",VFP_FLOAT ");

  if (ef & EF_ARM_MAVERICK_FLOAT)
    Print(L",MAVERICK_FLOAT ");

  Print(L"\r\n");

  /* MSB of flags conatins ARM EABI version */
  Print("ARM EABI\t= Version %d\n", (ef & EF_ARM_EABIMASK) >> 24);

  Print("\n");    /* End of ELF header */
}

EFI_FILE *load_file(EFI_FILE *directory, CHAR16 *path) {
  EFI_FILE *loaded_file = NULL;

  EFI_LOADED_IMAGE_PROTOCOL *loaded_image = NULL;
  uefi_call_wrapper(
      global_env->st->BootServices->HandleProtocol,
      3,
      global_env->handle,
      &gEfiLoadedImageProtocolGuid,
      (void **) &loaded_image
  );

  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file_system = NULL;
  uefi_call_wrapper(
      global_env->st->BootServices->HandleProtocol,
      3,
      loaded_image->DeviceHandle,
      &gEfiSimpleFileSystemProtocolGuid,
      (void **) &file_system
  );
  if (directory == NULL) {
    file_system->OpenVolume(file_system, &directory);
  }

  EFI_STATUS status = directory->Open(directory, &loaded_file, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
  if (status != EFI_SUCCESS) {
    Print(L"load file %s not success\r\n", path);
    return NULL;
  }
  return loaded_file;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  {
    // init env
    global_env->handle = ImageHandle;
    global_env->st = SystemTable;
  }
#if defined(_GNU_EFI)
  InitializeLib(ImageHandle, SystemTable);
#endif

  uefi_call_wrapper(global_env->st->ConOut->Reset, 2, global_env->st->ConOut, FALSE);
//  g_print(L"OS name: %s\r\n", os_name);
//  g_print(L"os name address: %lu\r\n", magic_number);
//  g_print(L"Hello, world!\r\n");
//  g_print(L"This is second line");
//  g_print(L".\r\n");
//  find_acpi2();
//  setup_uefi();
//  clean_log_cache();

  // read kernel file
  EFI_FILE *kernel = load_file(NULL, L"kernel.elf");
  Print(L"test\r\n");
  if (kernel == NULL) {
    Print(L"cannot find 'kernel.elf'\r\n");
  } else {
    Print(L"loaded kernel file\r\n");
    Elf64_Ehdr header;
    {
      UINTN file_info_size;
      EFI_FILE_INFO *file_info = NULL;
      kernel->GetInfo(kernel, &gEfiFileInfoGuid, &file_info_size, NULL);
      EFI_STATUS
          status = global_env->st->BootServices->AllocatePool(EfiLoaderData, file_info_size, (void **) &file_info);
      if (status != EFI_SUCCESS) {
        Print(L"allocate pool error\r\n");
      }
      status = kernel->GetInfo(kernel, &gEfiFileInfoGuid, &file_info_size, file_info);
      if (status != EFI_SUCCESS) {
        Print(L"unknown error\r\n");
      }
#ifdef _DEBUG
      Print(
          L"kernel file name: %s\r\n"
          L"  physical size: %d\r\n"
          L"  file size: %d\r\n"
          L"  size %d\r\n",
          file_info->FileName, file_info->PhysicalSize, file_info->FileSize, file_info->Size);
#endif
      UINTN size = sizeof(header);
      kernel->Read(kernel, &size, &header);
      Print(L"read file header\r\n");
    }
    if (CompareMem(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
        header.e_ident[EI_CLASS] != ELFCLASS64 ||
        header.e_ident[EI_DATA] != ELFDATA2LSB ||
        header.e_type != ET_EXEC ||
        header.e_machine != EM_X86_64 ||
        header.e_version != EV_CURRENT) {
      // error
      Print(L"kernel is not correct format\r\n");
    }
    Print(L"kernel is correct format\r\n");
#ifdef _DEBUG
//      print_elf_header64(header);
#endif
    // fixme: unknown bug
    Elf64_Phdr *phdrs;
    {
      EFI_STATUS status = kernel->SetPosition(kernel, header.e_phoff);
      if (status != EFI_SUCCESS) {
        Print(L"unknown error\r\n");
      }
      Print(L"1\r\n");
      UINTN size = header.e_phnum * header.e_phentsize;
      status = global_env->st->BootServices->AllocatePool(EfiLoaderData, size, (void **) &phdrs);
      if (status != EFI_SUCCESS) {
        Print(L"allocate pool error\r\n");
      }
      kernel->Read(kernel, &size, phdrs);
      Print(L"2\r\n");
    }
    for (Elf64_Phdr *phdr = phdrs;
         (char *) phdr < (char *) phdrs + header.e_phnum + header.e_phentsize;
         phdr = (Elf64_Phdr *) ((char *) phdrs + header.e_phentsize)
        ) {
      switch (phdr->p_type) {
        case PT_LOAD: {
          INTN pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
          Elf64_Addr segment = phdr->p_paddr;
          uefi_call_wrapper(global_env->st->BootServices->AllocatePages,
                            4,
                            AllocateAddress,
                            EfiLoaderData,
                            pages,
                            &segment);
          kernel->SetPosition(kernel, phdr->p_offset);
          UINTN size = phdr->p_filesz;
          kernel->Read(kernel, &size, (void *) &segment);
          break;
        }
      }
    }
    int (*kernel_start)() = ((__attribute__((sysv_abi)) int (*)()) header.e_entry);

    // jump to kernel
    int status = kernel_start();
    Print(L"kernel return status: %d\r\n", status);
  }

  UINTN Event;
  uefi_call_wrapper(global_env->st->ConIn->Reset, 2, global_env->st->ConIn, FALSE);
  uefi_call_wrapper(global_env->st->BootServices->WaitForEvent, 3, 1, &SystemTable->ConIn->WaitForKey, &Event);
#if defined(_DEBUG)
  // If running in debug mode, use the EFI shut down call to close QEMU
  uefi_call_wrapper(global_env->st->RuntimeServices->ResetSystem, 4, EfiResetShutdown, EFI_SUCCESS, 0, NULL);
#endif
  return 0;
}

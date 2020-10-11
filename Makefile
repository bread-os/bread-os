include .env
ARCH            = x64
# You can alter the subsystem according to your EFI binary target:
# 10 = EFI application
# 11 = EFI boot service driver
# 12 = EFI runtime driver
SUBSYSTEM       = 10

MINGW_HOST    	= w64
GNUEFI_ARCH   	= x86_64
GCC_ARCH      	= x86_64
QEMU_ARCH     	= x86_64
FW_BASE       	= OVMF
CROSS_COMPILE 	= $(GCC_ARCH)-$(MINGW_HOST)-mingw32-
EP_PREFIX     	=
CFLAGS        	= -m64 -mno-red-zone
LDFLAGS       	= -Wl,-dll -Wl,--subsystem,$(SUBSYSTEM)
FW_ARCH         = $(shell echo $(ARCH) | tr a-z A-Z)
FW_ZIP          = $(FW_BASE)-$(FW_ARCH).zip
GNUEFI_DIR      = $(CURDIR)/deps/gnu-efi
GNUEFI_LIBS     = lib

# If the compiler produces an elf binary, we need to fiddle with a PE crt0
ifneq ($(CRT0_LIBS),)
  CRT0_DIR      = $(GNUEFI_DIR)/$(GNUEFI_ARCH)/gnuefi
  LDFLAGS      += -L$(CRT0_DIR) -T $(GNUEFI_DIR)/gnuefi/elf_$(GNUEFI_ARCH)_efi.lds $(CRT0_DIR)/crt0-efi-$(GNUEFI_ARCH).o
  GNUEFI_LIBS  += gnuefi
endif

# SYSTEMROOT is only defined on Windows systems
ifneq ($(SYSTEMROOT),)
  QEMU          = "/c/Program Files/qemu/qemu-system-$(QEMU_ARCH)w.exe"
  # MinGW on Windows doesn't use (tuple)-ar but (tuple)-gcc-ar
  # so we remove the cross compiler tuple altogether
  CROSS_COMPILE =
else
  QEMU          = qemu-system-$(QEMU_ARCH) -nographic
endif

CC             	:= $(CROSS_COMPILE)gcc
OBJCOPY        	:= $(CROSS_COMPILE)objcopy
CFLAGS         	+= -fno-stack-protector -Wshadow -Wall -Wunused -Werror-implicit-function-declaration
CFLAGS         	+= -I$(GNUEFI_DIR)/inc -I$(GNUEFI_DIR)/inc/$(GNUEFI_ARCH) -I$(GNUEFI_DIR)/inc/protocol
CFLAGS			+= -I$(CURDIR)/include
CFLAGS         	+= -DCONFIG_$(GNUEFI_ARCH) -D__MAKEWITH_GNUEFI -DGNU_EFI_USE_MS_ABI
LDFLAGS        	+= -L$(GNUEFI_DIR)/$(GNUEFI_ARCH)/lib -e $(EP_PREFIX)efi_main
LDFLAGS        	+= -s -Wl,-Bsymbolic -nostdlib -shared
LIBS            = -lefi $(CRT0_LIBS)
KERNEL_FILES	= kernel/sys.c
KERNEL_FILES 	+= kernel/vmem_map.c

OBJS=$(KERNEL_FILES:.c=.o)

LIBS			+= $(OBJS)

PYTHON=python3

ifeq (, $(shell which $(CC)))
  $(error The selected compiler ($(CC)) was not found)
endif

GCCVERSION     := $(shell $(CC) -dumpversion | cut -f1 -d.)
GCCMINOR       := $(shell $(CC) -dumpversion | cut -f2 -d.)
GCCMACHINE     := $(shell $(CC) -dumpmachine)
GCCNEWENOUGH   := $(shell ( [ $(GCCVERSION) -gt "4" ]        \
                          || ( [ $(GCCVERSION) -eq "4" ]     \
                              && [ $(GCCMINOR) -ge "7" ] ) ) \
                        && echo 1)
ifneq ($(GCCNEWENOUGH),1)
  $(error You need GCC 4.7 or later)
endif

ifneq ($(GCC_ARCH),$(findstring $(GCC_ARCH), $(GCCMACHINE)))
  $(error The selected compiler ($(CC)) is not set for $(ARCH))
endif

.PHONY: all clean superclean
all: $(GNUEFI_DIR)/$(GNUEFI_ARCH)/lib/libefi.a main.efi

$(GNUEFI_DIR)/$(GNUEFI_ARCH)/lib/libefi.a:
	$(MAKE) -C$(GNUEFI_DIR) CROSS_COMPILE=$(CROSS_COMPILE) ARCH=$(GNUEFI_ARCH) $(GNUEFI_LIBS)

%.efi: %.o
	@echo  [LD]  $(notdir $@)
ifeq ($(CRT0_LIBS),)
	@$(CC) $(LDFLAGS) $< -o $@ $(LIBS)
else
	@$(CC) $(LDFLAGS) $< -o $*.elf $(LIBS)
	@$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel* \
	            -j .rela* -j .reloc -j .eh_frame -O binary $*.elf $@
	@rm -f $*.elf
endif

main.efi: $(OBJS)

%.o: %.c
	@echo  [CC]  $(notdir $@)
	$(CC) $(CFLAGS) -ffreestanding -c $< -o $@

qemu: CFLAGS += -D_DEBUG
qemu: all $(FW_BASE)_$(FW_ARCH).fd image/efi/boot/boot$(ARCH).efi
	@$(PYTHON) $(CURDIR)/build/start-qemu.py

image/efi/boot/boot$(ARCH).efi: main.efi
	mkdir -p image/efi/boot
	cp -f $< $@

$(FW_BASE)_$(FW_ARCH).fd:
	mv $(OVMF_PATH)/OVMF.fd $(CURDIR)/OVMF_X64.fd

clean:
	rm -f main.efi *.o kernel/*.o
	rm -rf image

superclean: clean
	$(MAKE) -C$(GNUEFI_DIR) ARCH=$(GNUEFI_ARCH) clean
	rm -f *.fd

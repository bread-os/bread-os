include .env
C_FLAGS=-nostdlib -shared -ffreestanding -fpic -fno-stack-protector -fshort-wchar -mno-red-zone -mgeneral-regs-only -mabi=ms -Wall -Wextra -Wpedantic -Wimplicit-function-declaration
EFI_MAIN=src/boot/efi_main.c
LD_SCRIPT=src/boot/elf_x86_64_efi.lds

INCLUDES_FILES=deps/edk2/MdePkg/Include/X64 deps/edk2/MdePkg/Include src/include
KERNEL_FILES=src/kernel/sys.c

INCLUDES=${foreach dir, $(INCLUDES_FILES), -I ${dir}}
OBJS=${OUTPUT_DIR}/$(notdir $(KERNEL_FILES:.c=.o))

TARGET=kernel

all: $(TARGET)

kernel: $(OBJS)
	gcc ${C_FLAGS} ${INCLUDES} -o ${OUTPUT_DIR}/main64.o -c $(EFI_MAIN) $(OBJS)
	gcc -nostdlib -shared -L /usr/lib -l:libgnuefi.a  -l:libefi.a -Wl,-T,${LD_SCRIPT},--build-id=none -Wl,-Bsymbolic -Wl,-znocombreloc -o ${OUTPUT_DIR}/kernel_x64.elf ${OUTPUT_DIR}/main64.o -lgcc
	objcopy -I elf64-x86-64 -O efi-app-x86_64 ${OUTPUT_DIR}/kernel_x64.elf ${OUTPUT_DIR}/BOOTX64.EFI

%.o : %.c
	gcc ${C_FLAGS} ${INCLUDES} -c $< -o ${OUTPUT_DIR}/$(notdir $@)

uefi: kernel
	bash build/make-uefi.sh

start: all uefi
	qemu-system-x86_64 -cpu qemu64 \
		-cpu qemu64 \
		-bios ${OVMF_PATH}/OVMF.fd \
		-drive if=ide,format=raw,file=${OUTPUT_DIR}/fat.img \
  		-net none

.PNONY: clean
clean:
	@echo "remove all files in ${OUTPUT_DIR}"
	@-rm -rf ${OUTPUT_DIR}/*

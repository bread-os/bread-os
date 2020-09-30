include .env
C_FLAGS=-ffreestanding -fpic -fno-stack-protector -fshort-wchar -mno-red-zone -mgeneral-regs-only -mabi=ms -Wall -Wextra -Wpedantic
INCLUDES=deps/edk2/MdePkg/Include/X64 deps/edk2/MdePkg/Include
EFI_MAIN=src/boot/efi_main.c
LD_SCRIPT=src/boot/elf_x86_64_efi.lds

all:
	@make kernel
	@make uefi

kernel:
	@gcc ${C_FLAGS} ${foreach dir, $(INCLUDES), -I ${dir}} -o ${OUTPUT_DIR}/main64.o -c ${EFI_MAIN}
	@gcc -nostdlib -shared -Wl,-T,${LD_SCRIPT},--build-id=none -Wl,-Bsymbolic -Wl,-znocombreloc -o ${OUTPUT_DIR}/kernel_x64.elf ${OUTPUT_DIR}/main64.o -lgcc
	@objcopy -I elf64-x86-64 -O efi-app-x86_64 ${OUTPUT_DIR}/kernel_x64.elf ${OUTPUT_DIR}/BOOTX64.EFI

uefi: kernel
	@bash build/make-uefi.sh

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

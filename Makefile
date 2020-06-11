OUTPUT_DIR=out

all: boot.bin boot.cpp

boot.bin:
	nasm src/boot/boot.asm \
	-f elf64 \
	-o ${OUTPUT_DIR}/boot.bin \
	-l ${OUTPUT_DIR}/boot.lst

boot.cpp: boot.bin
	g++ \
	-nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -Werror \
	-m64 src/boot/boot.cpp ${OUTPUT_DIR}/boot.bin \
	-o ${OUTPUT_DIR}/kernel.bin \
	-T src/boot/linker.ld

image: all
	dd if=${OUTPUT_DIR}/kernel.bin of=${OUTPUT_DIR}/kernel.img bs=512 count=2 conv=notrunc

clean:
	rm -rf ${OUTPUT_DIR}/*
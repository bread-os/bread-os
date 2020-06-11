OUTPUT_DIR=out

all:
	mkdir -p ${OUTPUT_DIR}
	make boot.bin
	make boot.cpp

# todo: support 64 bit?
boot.bin:
	nasm src/boot/boot.asm \
	-f elf32 \
	-o ${OUTPUT_DIR}/boot.bin \
	-l ${OUTPUT_DIR}/boot.lst

# -S -masm=intel x.c -o ${OUTPUT_DIR}/kernel.s
boot.cpp: boot.bin
	g++ \
	-nostdlib -ffreestanding -std=c++11 -fno-pie -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -Werror \
	-m32 src/print.h src/boot/boot.cpp ${OUTPUT_DIR}/boot.bin \
	-o ${OUTPUT_DIR}/kernel.bin \
	-T src/boot/linker.ld

image: all
	dd if=${OUTPUT_DIR}/kernel.bin of=${OUTPUT_DIR}/kernel.img bs=512 count=2 conv=notrunc

clean:
	rm -rf ${OUTPUT_DIR}/*
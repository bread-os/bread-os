include .env
CXX=g++
CXX_FLAGS= \
	-g -nostdinc -nostdlib -nostartfiles -fno-stack-protector -fno-threadsafe-statics -ffreestanding -fno-pie -fno-exceptions -fno-rtti -mcmodel=large \
	-std=c++17 -mno-red-zone \
	-Wall

KERNEL_OBJECTS=bmalloc.o bprint.o util.o
KERNEL_INCLUDE=src/include deps/bootboot

all:
	mkdir -p ${OUTPUT_DIR}
	make kernel

uefi:
	sh build/make-uefi.sh

start: all uefi
	qemu-system-x86_64 -cpu qemu64 \
		-cpu qemu64 -bios ${OVMF_PATH} \
		-drive file=${OUTPUT_DIR}/uefi.img,if=ide \
  		-net none

kernel: ${KERNEL_OBJECTS} boot.o
	cd font && ld -r -b binary -o font.o font.psf

	ld -nostdinc -nostdlib \
	-T src/boot/linker.ld ${OUTPUT_DIR}/boot.o ${OUTPUT_DIR}/bmalloc.o ${OUTPUT_DIR}/bprint.o ${OUTPUT_DIR}/util.o font/font.o \
	-o ${OUTPUT_DIR}/kernel.elf

boot.o: ${KERNEL_OBJECTS}
	${CXX} ${CXX_FLAGS} -c src/boot/boot.cpp -o ${OUTPUT_DIR}/boot.o ${addprefix -I,${KERNEL_INCLUDE}}

${KERNEL_OBJECTS}: %.o: src/kernel/%.cpp
	${CXX} ${CXX_FLAGS} -c $< -o ${OUTPUT_DIR}/$@ ${addprefix -I,${KERNEL_INCLUDE}}

.PNONY: clean
clean:
	-rm -rf ${OUTPUT_DIR}/*

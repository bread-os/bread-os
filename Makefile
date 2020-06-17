ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
OUTPUT_DIR=out
OVMF_PATH=deps/edk2/Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd
DISKSIZE=128
BOOTSIZE=16384
BOOTTYPE=16
CXX_FLAGS= \
	-nostdinc -nostdlib -nostartfiles -fno-stack-protector -fno-threadsafe-statics -ffreestanding -fno-pie -fno-exceptions -fno-rtti \
	-std=c++17 -mno-red-zone \
	-Wall -Wextra -Werror

all:
	mkdir -p ${OUTPUT_DIR}
	make boot.cpp

uefi:
	sh build/make-uefi.sh

start: all uefi
	qemu-system-x86_64 -cpu qemu64 \
		-cpu qemu64 -bios ${OVMF_PATH} \
		-drive file=${OUTPUT_DIR}/uefi.img,if=ide \
  		-net none

boot.cpp: bprint.cpp util.cpp
	g++ ${CXX_FLAGS} -c src/boot/boot.cpp -o ${OUTPUT_DIR}/boot.o \
	-I src/include \
	-I deps/bootboot

	ld -r -b binary -o ${OUTPUT_DIR}/font.o deps/bootboot/mykernel/font.psf
	ld -nostdinc -nostdlib \
	-T src/boot/linker.ld \
	${OUTPUT_DIR}/font.o ${OUTPUT_DIR}/boot.o ${OUTPUT_DIR}/bprint.o ${OUTPUT_DIR}/util.o \
	-o ${OUTPUT_DIR}/kernel.elf

bprint.cpp:
	g++ ${CXX_FLAGS} -c src/kernel/bprint.cpp -o ${OUTPUT_DIR}/bprint.o \
	-I src/include \
	-I deps/bootboot

util.cpp:
	g++ ${CXX_FLAGS} -c src/kernel/util.cpp -o ${OUTPUT_DIR}/util.o \
	-I src/include \
	-I deps/bootboot

clean:
	rm -rf ${OUTPUT_DIR}/*

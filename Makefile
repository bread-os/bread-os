OUTPUT_DIR=out
OVMF_PATH=deps/edk2/Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd
CXX=g++
CXX_FLAGS= \
	-nostdinc -nostdlib -nostartfiles -fno-stack-protector -fno-threadsafe-statics -ffreestanding -fno-pie -fno-exceptions -fno-rtti \
	-std=c++17 -mno-red-zone \
	-Wall -Wextra -Werror

KERNEL_OBJECTS=bprint.o util.o
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

kernel: boot.o ${KERNEL_OBJECTS}
	ld -r -b binary -o ${OUTPUT_DIR}/font.o deps/bootboot/mykernel/font.psf
	ld -nostdinc -nostdlib \
	-T src/boot/linker.ld \
	${OUTPUT_DIR}/font.o ${OUTPUT_DIR}/boot.o ${OUTPUT_DIR}/bprint.o ${OUTPUT_DIR}/util.o \
	-o ${OUTPUT_DIR}/kernel.elf

boot.o: ${KERNEL_OBJECTS}
	${CXX} ${CXX_FLAGS} -c src/boot/boot.cpp -o ${OUTPUT_DIR}/boot.o ${addprefix -I,${KERNEL_INCLUDE}}

${KERNEL_OBJECTS}: %.o: src/kernel/%.cpp
	${CXX} ${CXX_FLAGS} -c $< -o out/$@ ${addprefix -I,${KERNEL_INCLUDE}}

.PNONY: clean
clean:
	-rm -rf ${OUTPUT_DIR}/*

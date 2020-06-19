include .env

all: font.o
	@mkdir -p ${CMAKE_DEBUG_DIR}

kernel: font.o
	@mkdir -p ${CMAKE_DEBUG_DIR}
	@cd ${CMAKE_DEBUG_DIR} && cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" .. && make

uefi: kernel
	@sh build/make-uefi.sh

start: all uefi
	qemu-system-x86_64 -cpu qemu64 \
		-cpu qemu64 -bios ${OVMF_PATH} \
		-drive file=${OUTPUT_DIR}/uefi.img,if=ide \
  		-net none

font.o:
	@cd font && ld -r -b binary -o font.o font.psf
	@echo "out/font.o"
	@readelf -s font/font.o

.PNONY: clean
clean:
	@echo "remove all files in ${OUTPUT_DIR}"
	@-rm -rf ${OUTPUT_DIR}/*
	@echo "remove all files in ${CMAKE_DEBUG_DIR}"
	@-rm -rf ${CMAKE_DEBUG_DIR}/*

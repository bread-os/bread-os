cmake_minimum_required(VERSION 3.10)
project(bread-os)

SET(CMAKE_CXX_FLAGS "-nostdinc -nostdlib -nostartfiles \
    -fno-stack-protector -fno-threadsafe-statics -ffreestanding -fno-pie -no-pie -fno-exceptions -fno-rtti -mcmodel=kernel \
	-mno-red-zone \
	-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_HOME_DIRECTORY}/src/boot/linker.ld")

file(GLOB KERNEL_OBJECTS src/kernel/*.cpp)

add_executable(kernel.elf src/boot/boot.cpp font/font.o ${KERNEL_OBJECTS})
include_directories(BEFORE
        src/include deps/bootboot)

add_custom_command(TARGET kernel.elf POST_BUILD
        COMMAND objcopy --only-keep-debug kernel.elf kernel.sym
        COMMAND objdump -C -d -S --no-show-raw-insn kernel.elf > kernel.lst
        COMMAND objdump -C -d kernel.elf > kernel.asm
        COMMAND nm -C kernel.elf | sort | uniq > kernel.sym.txt
        COMMAND strip kernel.elf
        )

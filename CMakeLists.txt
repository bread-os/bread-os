cmake_minimum_required(VERSION 3.16)
project(bread_os)
set(CMAKE_CXX_STANDARD 20)

# ASM
set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS nasm asm S)


set(BOOT_FILES src/kernel/bios.asm)
option(BUILD_KERNEL "build kernel" OFF)

if(BUILD_KERNEL)

endif()

# CORE
add_library(bread_os src/library.cpp src/library.h)
cmake_minimum_required(VERSION 3.16)
project(bread_os)
set(CMAKE_CXX_STANDARD 20)

# fixme: support link with asm
add_library(bread_os_kernel src/boot/boot.cpp)
# tip: for now, cmake only for CLion tooltip
#   we use make on linux
cmake_minimum_required(VERSION 3.16)
project(bread_os)
set(CMAKE_CXX_STANDARD 20)

add_library(bread_os_kernel src/boot/boot.cpp src/print.h)
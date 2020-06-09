exports.CC_COMPILER = 'gcc'
exports.CXX_COMPILER = 'g++'
exports.OUTPUT_DIR = 'out'

exports.BOOTLOADER_FILE = 'src/boot/boot.cpp'
exports.BOOTLOADER_OUTPUT = `${exports.OUTPUT_DIR}/boot.bin`

exports.LINK_FILE = 'src/boot/linker.ld'

exports.KERNEL_OUTPUT = `${exports.OUTPUT_DIR}/kernel.bin`
exports.KERNEL_BUILD_ARGS = `-m64 ${exports.BOOTLOADER_FILE} ${exports.BOOTLOADER_OUTPUT} -o ${exports.KERNEL_OUTPUT} -nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -Werror -T ${exports.LINK_FILE}`

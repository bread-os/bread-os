module.exports = (debug = true) => {
  const e = {}
  e.CC_COMPILER = 'gcc'
  e.CXX_COMPILER = 'g++'
  e.CXX_FLAGS = '-m64 -nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -Werror'
  e.OUTPUT_DIR = 'out'

  e.BOOTLOADER_FILE = 'src/boot/boot.cpp'
  e.BOOTLOADER_OUTPUT = `${e.OUTPUT_DIR}/boot.bin`

  e.LINK_FILE = 'src/boot/linker.ld'

  e.KERNEL_OUTPUT = `${e.OUTPUT_DIR}/kernel.bin`
  e.KERNEL_BUILD_ARGS =
    `${e.CXX_FLAGS} ` +
    `${debug ? '-g' : ''} ${e.BOOTLOADER_FILE} ${e.BOOTLOADER_OUTPUT} ` +
    `-o ${e.KERNEL_OUTPUT} ` +
    `-T ${e.LINK_FILE}`
  return e
}

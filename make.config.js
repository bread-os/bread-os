module.exports = (debug = true) => {
  const e = {}
  e.CC_COMPILER = 'gcc'
  e.CXX_COMPILER = 'g++'
  e.OUTPUT_DIR = 'out'

  e.BOOTLOADER_FILE = 'src/boot/boot.cpp'
  e.BOOTLOADER_OUTPUT = `${e.OUTPUT_DIR}/boot.bin`

  e.LINK_FILE = 'src/boot/linker.ld'

  e.KERNEL_OUTPUT = `${e.OUTPUT_DIR}/kernel.bin`
  e.KERNEL_BUILD_ARGS =
    // todo: does '-g' param for single file or all files?
    `-m64 ${debug ? '-g' : ''} ${e.BOOTLOADER_FILE} ${debug ? '-g' : ''} ${e.BOOTLOADER_OUTPUT} ` +
    `-o ${e.KERNEL_OUTPUT} ` +
    '-nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -Werror ' +
    `-T ${e.LINK_FILE}`
  return e
}

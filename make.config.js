module.exports = (debug = true) => {
  const e = {}
  e.CC_COMPILER = 'gcc'
  e.CXX_COMPILER = 'g++'
  e.CXX_FLAGS = '-m64 -nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -Werror'
  e.OUTPUT_DIR = 'out'

  e.BOOTLOADER_ENTRY = 'src/boot/boot.asm'
  e.BOOTLOADER_ENTRY_OUTPUT = `${e.OUTPUT_DIR}/boot.bin`
  e.BOOTLOADER_ENTRY_LST = `${e.OUTPUT_DIR}/boot.lst`
  e.BOOTLOADER_ENTRY_ARGS =
    `${e.BOOTLOADER_ENTRY} ` +
    `-o ${e.BOOTLOADER_ENTRY_OUTPUT} ` +
    `${debug ? `-l ${e.BOOTLOADER_ENTRY_LST}` : ''} ` +
    '-f elf64'
  e.BOOTLOADER_FILE = 'src/boot/boot.cpp'
  e.BOOTLOADER_OUTPUT = `${e.OUTPUT_DIR}/boot.bin`

  e.LINK_FILE = 'src/boot/linker.ld'

  e.KERNEL_OUTPUT = `${e.OUTPUT_DIR}/kernel.bin`
  e.KERNEL_BUILD_ARGS =
    `${e.CXX_FLAGS} ` +
    `${debug ? '-g ' : ''}${e.BOOTLOADER_FILE} ${e.BOOTLOADER_OUTPUT} ` +
    `-o ${e.KERNEL_OUTPUT} ` +
    `-T ${e.LINK_FILE}`

  e.IMAGE_OUTPUT = `${e.OUTPUT_DIR}/kernel.img`
  // tip: quiet mode '-q'
  //  but if IMAGE_OUTPUT exists it will fall back to interactive mode
  e.IMAGE_CREATE_ARGS = `-q -fd="1.44M" -mode=create ${e.IMAGE_OUTPUT}`

  e.DD_ARGS = `if=${e.KERNEL_OUTPUT} of=${e.IMAGE_OUTPUT} bs=512 count=1 conv=notrunc`

  return e
}

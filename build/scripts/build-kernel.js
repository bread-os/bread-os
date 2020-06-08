const assert = require('assert')
const { spawnSync } = require('child_process')
const { basename, extname } = require('path')
const data = require('../data')

const exitIfString = (str) => {
  if (str) {
    console.error(str)
    process.exit(1)
  }
}

module.exports = () => {
  console.log('build kernel: start')
  const { boot } = data
  for (const { entry: bootFile, format } of boot) {
    const filename = basename(bootFile, extname(bootFile))
    // fixme: use spawn instead of spawnSync?
    const cp = spawnSync('nasm', [
      `${bootFile}`,
      '-o',
      `out/${filename}.bin`,
      '-l', // todo: user can trigger if generate .lst files
      `out/${filename}.lst`,
      '-f', format
    ])
    assert.ifError(cp.error)
    exitIfString(cp.stderr.toString('utf-8').trim())
    exitIfString(cp.stdout.toString('utf-8').trim())
  }
  console.log('build kernel: done')
}

/***
 * todo:
 *  - support linker
 *      g++ -m64 src/boot/boot.cpp out/boot.bin -o kernel.bin -nostdlib -ffreestanding -std=c++11 -mno-red-zone
 *          -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -Werror -T src/boot/linker.ld
 */

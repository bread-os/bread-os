const assert = require('assert')
const { spawnSync } = require('child_process')
const { basename, extname } = require('path')
const data = require('../data')

module.exports = () => {
  console.log('build kernel: start')
  const { boot } = data
  for (const bootFile of boot) {
    const filename = basename(bootFile, extname(bootFile))
    // fixme: use spawn instead of spawnSync?
    const cp = spawnSync('nasm', [
      `${boot}`,
      '-o',
      `out/${filename}.bin`,
      '-l', // todo: user can trigger if generate .lst files
      `out/${filename}.lst`,
      '-f', 'bin'
    ])
    assert.ifError(cp.error)
    if (cp.stdout.toString('utf-8').trim() !== '') {
      // todo: abstract this scope
      console.error(cp.stdout.trim().toString('utf-8'))
      process.exit(1)
    }
  }
  console.log('build kernel: done')
}

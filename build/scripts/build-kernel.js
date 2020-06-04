const assert = require('assert')
const { spawnSync } = require('child_process')
const { resolve, basename, extname } = require('path')
const { statSync, mkdirSync, rmdirSync, existsSync } = require('fs')
const data = require('../data')

module.exports = () => {
  console.log('build kernel: start')
  const { boot } = data
  const outputDir = resolve(process.cwd(), 'out')
  if (existsSync(outputDir)) {
    const outputDirStat = statSync(outputDir)
    if (!outputDirStat.isDirectory()) {
      if (outputDirStat.isFile()) {
        // remove 'out' if it's file
        rmdirSync(outputDir)
      }
      mkdirSync(outputDir)
    }
  } else {
    mkdirSync(outputDir)
  }
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
  }
  console.log('build kernel: done')
}

const { spawn } = require('child_process')
const { resolve } = require('path')
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
  // fixme: error handle
  // fixme: multi input
  const cp = spawn(
    'nasm',
    [
      `${boot}`,
      '-o',
      'out/kernel.bin',
      '-l', // todo: user can trigger if generate .lst files
      'out/kernel.lst',
      '-f', 'bin'
    ],
    {
      cwd: process.cwd(),
      env: process.env
    }
  )
  cp.on('error', console.error)
  console.log('build kernel: done')
}

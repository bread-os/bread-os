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
  const cp = spawn(
    'nasm',
    [
      ...boot.map(item => item),
      '-o',
      ...boot.map(item => 'out/kernel.bin'),
      '-l', // todo: user can trigger if generate .lst files
      ...boot.map(item => 'out/kernel.lst'),
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

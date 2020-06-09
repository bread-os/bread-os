const assert = require('assert')
const { Signale } = require('signale')
const { spawnSync } = require('child_process')
const { platform } = require('os')
const { basename, extname } = require('path')
const { getConfig } = require('../util')
const data = require('../data')
const signal = new Signale({
  scope: 'build-kernel'
})

const exitIfError = (cp) => {
  assert.ifError(cp.error)
  exitIfString(cp.stderr.toString('utf-8').trim())
  exitIfString(cp.stdout.toString('utf-8').trim())

  function exitIfString (str) {
    if (str) {
      signal.fatal(str)
      process.exit(1)
    }
  }
}

module.exports = () => {
  signal.pending('Start')
  const { boot } = data
  {
    const [{ entry: bootFile, format }] = boot.splice(0, 1)
    const filename = basename(bootFile, extname(bootFile))
    const kernelOutput = `out/${filename}.bin`
    // fixme: use spawn instead of spawnSync?
    const cp = spawnSync('nasm', [
      `${bootFile}`,
      '-o',
      kernelOutput,
      '-l', // todo: user can trigger if generate .lst files
      `out/${filename}.lst`,
      '-f', format
    ])
    exitIfError(cp)
  }
  // link
  {
    const config = getConfig()
    const CXX_COMPILER = config.CXX_COMPILER

    let command = CXX_COMPILER
    let args = []
    if (platform() === 'win32') {
      command = 'wsl'
      args = [CXX_COMPILER]
    }
    args = [...args, ...config.KERNEL_BUILD_ARGS.split(' ')]
    const cp = spawnSync(
      command,
      args,
      {
        env: process.env,
        cwd: process.cwd()
      }
    )
    exitIfError(cp)
  }
  signal.success('Done')
}

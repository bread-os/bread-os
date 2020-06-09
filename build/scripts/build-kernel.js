const { Signale } = require('signale')
const { spawnSync } = require('child_process')
const { platform } = require('os')
const { basename, extname } = require('path')
const { getConfig, exitIfError } = require('../util')
const data = require('../data')
const signal = new Signale({
  scope: 'build-kernel'
})

module.exports = () => {
  signal.pending('start')
  const config = getConfig()
  signal.debug(
    'DEBUG mode: %s',
    process.env.NODE_ENV === 'debug' ? 'on' : 'off'
  )
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
    signal.success('boot build done')
  }
  // link
  {
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
    signal.success('kernel build done')
  }
  signal.success('all done')
}

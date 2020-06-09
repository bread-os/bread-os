const { Signale } = require('signale')
const { spawnSync } = require('child_process')
const { basename, extname } = require('path')
const { getConfig, exitIfError, compatibleSpawnParams } = require('../util')
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
    const { command, args } =
      compatibleSpawnParams(
        config.CXX_COMPILER,
        config.KERNEL_BUILD_ARGS.split(' ')
      )
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

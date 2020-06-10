const { Signale } = require('signale')
const { spawnSync } = require('child_process')
const { getConfig, exitIfError, compatibleSpawnParams } = require('../util')
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
  // build boot file
  {
    // fixme: use spawn instead of spawnSync?
    const cp = spawnSync(
      'nasm',
      config.BOOTLOADER_ENTRY_ARGS.split(' '),
      {
        encoding: 'utf-8'
      }
    )
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
        cwd: process.cwd(),
        encoding: 'utf-8'
      }
    )
    exitIfError(cp)
    signal.success('kernel build done')
  }
  // make image
  {
    /**
     * fixme: use typescript?
     * @type {ChildProcess}
     */
    const cp = spawnSync(
      'bximage',
      config.IMAGE_CREATE_ARGS.split(' '),
      {
        encoding: 'utf-8',
        stdio: 'inherit'
      }
    )
    exitIfError(cp)
    signal.success('bximage done')
  }
  // todo: write kernel to image
  // eslint-disable-next-line no-lone-blocks
  {

  }
  signal.success('all done')
}

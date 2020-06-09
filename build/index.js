const { program } = require('commander')
const { checkDependenciesExists } = require('./util')

checkDependenciesExists()

program.allowUnknownOption(true)
  .option('-d --debug', 'turn on debug mode', () => {
    process.env.DEBUG = 'true'
  })
  .option('--build-kernel', 'build kernel', require('./scripts/build-kernel'))
  .option('-p --print-config [args]', 'print config', require('./scripts/print-config'))
  .option('--clean', 'clean output', require('./scripts/clean'))

program.parse(process.argv)

const { program } = require('commander')
const { checkDependenciesExists, getConfig } = require('./util')

checkDependenciesExists()

program.allowUnknownOption(true)
  .option('-d --debug', 'turn on debug mode', () => {
    process.env.DEBUG = 'true'
  })
  .option('--build-kernel', 'build kernel', require('./scripts/build-kernel'))
  .option('--print-config', 'print config', () => console.log(getConfig()))
  .option('--clean', 'clean output', require('./scripts/clean'))

program.parse(process.argv)

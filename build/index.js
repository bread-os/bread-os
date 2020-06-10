const { program } = require('commander')
const { resolve } = require('path')
// tip: load env file before every script
require('dotenv').config({
  path: resolve(process.cwd(), '.env')
})
const { checkDependenciesExists } = require('./util')

checkDependenciesExists()

program.allowUnknownOption(true)
  .option('--build-kernel', 'build kernel', require('./scripts/build-kernel'))
  .option('-p --print-config [args]', 'print config',
    require('./scripts/print-config'))
  .option('--clean', 'clean output', require('./scripts/clean'))

program.parse(process.argv)

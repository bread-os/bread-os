// load env files
require('dotenv').config()
const { ok } = require('assert')
const { program } = require('commander')

// make sure this is in node make env
ok(process.env.NODE_MAKE)

program.allowUnknownOption(true)
  .option('-d --debug', 'turn on debug mode', () => {
    process.env.DEBUG = 'true'
  })
  .option('--build-kernel', 'build kernel', require('./scripts/build-kernel'))
  .option('--clean', 'clean output', require('./scripts/clean'))

program.parse(process.argv)

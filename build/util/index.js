const signal = require('signale')
const assert = require('assert')
const { statSync, existsSync, mkdirSync, unlinkSync } = require('fs')
const { resolve } = require('path')

const cwd = process.cwd()
const outputDir = resolve(cwd, 'out')

// tip: lazy load outputDirStat
let outputDirStat
const lazyLoadOutputDirStat = () => {
  if (!outputDirStat) {
    outputDirStat = statSync(outputDir)
  }
  return outputDirStat
}

exports.checkDependenciesExists = () => {
  // todo: use async/await syntax
  if (existsSync(outputDir)) {
    const outputDirStat = lazyLoadOutputDirStat()
    if (!outputDirStat.isDirectory()) {
      if (outputDirStat.isFile()) {
        unlinkSync(outputDir)
      }
      mkdirSync(outputDir)
    }
  } else {
    mkdirSync(outputDir)
  }
}

/**
 *
 * @param cp {SpawnSyncReturns}
 */
exports.exitIfError = (cp) => {
  assert.ifError(cp.error)
  exitIfString(cp.stderr.toString('utf-8').trim())
  exitIfString(cp.stdout.toString('utf-8').trim())

  function exitIfString (str) {
    if (str) {
      signal.fatal(str)
      signal.debug(exports.getConfig())
      process.exit(1)
    }
  }
}

let config = null
let current = process.env.NODE_ENV
/**
 * Dynamic load the config file when `process.env.DEBUG` changed.
 * @param filename {string}
 * @return {object}
 */
exports.getConfig = (filename = 'make.config.js') => {
  if (currentChanged()) {
    return tryLoadConfig()
  }
  if (config) return config
  return tryLoadConfig()
  // end

  function currentChanged () {
    if (process.env.DEBUG !== current) {
      current = process.env.NODE_ENV
      return true
    }
    return false
  }

  function tryLoadConfig () {
    try {
      config = require(resolve(cwd, filename))(current === 'debug')
    } catch (_) {
      config = {}
    }
    return config
  }
}

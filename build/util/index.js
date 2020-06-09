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

let config = null
/**
 *
 * @param filename {string}
 * @return {object}
 */
exports.getConfig = (filename = 'make.config.js') => {
  if (config) return config
  try {
    config = require(resolve(cwd, filename))
  } catch (_) {
    config = {}
  }
  return config
}

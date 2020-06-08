const { statSync, existsSync, mkdirSync, unlinkSync } = require('fs')
const { resolve } = require('path')

const cwd = process.cwd()
const outputDir = resolve(cwd, 'out')

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

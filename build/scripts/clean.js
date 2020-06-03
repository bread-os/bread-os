const fs = require('fs')
const { resolve } = require('path')

module.exports = () => {
  fs.rmdirSync(resolve(process.cwd(), 'out'), { recursive: true })
}

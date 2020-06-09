const { getConfig } = require('../util')

module.exports = (variables) => {
  variables = variables.split(',').map(value => value.toUpperCase())
  const config = getConfig()
  if (variables.length > 0) {
    for (const variable of variables) {
      if (typeof config[variable] === 'string') {
        console.log(`${variable}: '${config[variable]}'`)
      }
    }
  } else {
    console.log(config)
  }
}

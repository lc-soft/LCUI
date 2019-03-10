const Builder = require('./builder')
const config = require('./build-win-dist.config')
const pkg = require('../package.json')

const builder = new Builder(pkg, config)

builder.run()

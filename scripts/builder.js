const fs = require('fs')
const path = require('path')
const mkdirp = require('mkdirp')
const archiver = require('archiver')

class Builder {
  constructor(pkg, config) {
    this.config = config
    this.target = `${pkg.name}-${pkg.version}`
    this.zipfile = path.join(config.output, `${this.target}.zip`)
  }

  run() {
    const config = this.config

    console.log(`build package: ${this.zipfile}`)
    mkdirp.sync(path.join(config.output, this.target))
    Object.keys(config.entry).forEach((name) => {
      const entry = config.entry[name]
      
      if (!entry.subdir) {
        this.buildEntry(entry)
        return
      }
      config.platform.forEach((platform) => {
        config.arch.forEach((arch) => {
          config.mode.forEach((mode) => {
            this.buildEntry(
              entry,
              entry.subdir.replace('${arch}', arch)
                .replace('${platform}', platform)
                .replace('${mode}', mode)
            )
          })
        })
      })
    })
    this.pack()
  }

  pack() {
    const output = fs.createWriteStream(this.zipfile)
    const archive = archiver('zip')

    console.log(`create: ${this.zipfile}`)
    archive.pipe(output)
    archive.directory(path.join(this.config.output, this.target), this.target)
    archive.finalize()
  }

  copyFiles(entry, input, output) {
    mkdirp.sync(output)
    fs.readdirSync(input).forEach((file) => {
      const srcPath = path.join(input, file)
      const destPath = path.join(output, file)
      const stats = fs.statSync(srcPath)

      if (stats.isDirectory()) {
        this.copyFiles(entry, srcPath, destPath)
        return
      }
      if (entry.regex && !entry.regex.test(srcPath)) {
        return
      }
      if (entry.extensions.some(e => srcPath.endsWith('.' + e))) {
        console.log(`create: ${destPath}`)
        fs.copyFileSync(srcPath, destPath)
      }
    })
  }

  buildEntry(entry, subdir) {
    let input = entry.input
    let output = path.join(this.config.output, this.target, entry.output)

    if (subdir) {
      input = input.replace('${subdir}', subdir)
      output = output.replace('${subdir}', subdir)
    }
    this.copyFiles(entry, input, output)
  }
}

module.exports = Builder

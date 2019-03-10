module.exports = {
  output: 'dist',
  entry: {
    lib: {
      subdir: '${arch}-${platform}/${mode}',
      input: 'bin/${subdir}',
      output: 'lib/${subdir}',
      regex: /(LCUI\.)|(LCUIMain\.)/,
      extensions: ['pdb', 'dll', 'lib']
    },
    bin: {
      subdir: '${arch}-${platform}/${mode}',
      input: 'bin/${subdir}',
      output: 'bin',
      extensions: ['exe']
    },
    binDeps: {
      subdir: '${arch}-${platform}/${mode}',
      input: 'bin/${subdir}',
      output: 'bin',
      extensions: ['dll']
    },
    assets: {
      input: 'test',
      output: 'bin',
      extensions: ['xml', 'css']
    },
    headers: {
      input: 'include',
      output: 'include',
      extensions: ['h']
    }
  },
  arch: [
    'x64'
  ],
  mode: [
    'Debug',
    'Release'
  ],
  platform: [
    'windows'
  ]
}

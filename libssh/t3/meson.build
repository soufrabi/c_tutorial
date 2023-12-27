project('tyrant','c')

sshdep = dependency('libssh')

sourcefiles=files('main.c')

executable('app',sourcefiles,dependencies:sshdep, install:false)

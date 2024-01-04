project('test','c')

sshdep = dependency('libssh')
gtkdep = dependency('gtk4')


sourcefiles=files('main.c')

executable('app',sourcefiles,dependencies:[sshdep,gtkdep], install:false)

project('poule', 'c', default_options : ['c_std=c99'])
thread_dep = dependency('threads')
subdir('src')
executable('example', sources, dependencies : thread_dep)

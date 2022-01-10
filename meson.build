project(
  'poule',
  'c',
  default_options : ['c_std=c99'],
  # c_args : ['Wall', 'Wextra']
)
add_global_arguments('-g', language : 'c')
incdir = include_directories('include')
thread_dep = dependency('threads')
subdir('src')
lib = library('poule', sources, dependencies : thread_dep, include_directories : incdir)
subdir('examples')

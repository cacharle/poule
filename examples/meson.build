executable('map', 'map.c', link_with : lib, include_directories : incdir)

cc = meson.get_compiler('c')
math_dep = cc.find_library('m', required : true)
executable(
  'pi',
  'pi.c',
  dependencies : math_dep,
  link_with : lib,
  include_directories : incdir
)

sources = files(
  'test_queue.c'
)
cc = meson.get_compiler('c')
criterion_dep = cc.find_library('criterion', required : true)
test_exec = executable(
  'test_criterion',
  sources,
  link_with : lib,
  include_directories : incdir,
  dependencies : criterion_dep
)
test('test', test_exec)

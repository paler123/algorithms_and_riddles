islands_sources = [
  'islands.cpp',
  'dynamic_matrix.cpp'
]

islands_lib = library(
  'islands_lib',
  islands_sources,
  cpp_args : used_warnings,
  include_directories : islands_includes,
  install : true
)

islands_private_includes = include_directories('.')

islands_dep = declare_dependency(
  link_with : islands_lib,
  include_directories : islands_includes
)

regexes_sources = [
  'matcher.cpp',
  'to_intermediate.cpp',
  'pattern_parser.cpp'
]

regexes_lib = library(
  'regexes_lib',
  regexes_sources,
  cpp_args : used_warnings,
  include_directories : regexes_includes,
  install : true
)

regexes_private_includes = include_directories('.')

regexes_dep = declare_dependency(
  link_with : regexes_lib,
  include_directories : regexes_includes
)

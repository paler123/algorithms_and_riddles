max_matrix_sum_sources = [
  'array2d.cpp',
  'matrix_type_traits.cpp',
  'max_sum_solution.cpp'
]

max_matrix_sum_lib = library(
  'max_matrix_sum_lib',
  max_matrix_sum_sources,
  cpp_args : used_warnings,
  include_directories : max_matrix_sum_includes,
  install : true
)

max_matrix_sum_dep = declare_dependency(
  link_with : max_matrix_sum_lib,
  include_directories : max_matrix_sum_includes
)

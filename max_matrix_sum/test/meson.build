max_matrix_sum_ut_sources = [
    'solver.cpp',
    'solver_path.cpp',
    'tests.cpp'
]

max_matrix_sum_test_exe = executable(
    'max_matrix_sum_ut',
    max_matrix_sum_ut_sources,
    cpp_args : used_warnings,
    dependencies : [max_matrix_sum_dep, catch2_dep]
)


test('max_matrix_sum_ut', max_matrix_sum_test_exe)

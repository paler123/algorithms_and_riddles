islands_ut_sources = [
    'tests.cpp',
    'islands.cpp'
]

islands_test_exe = executable(
    'islands_ut',
    islands_ut_sources,
    cpp_args : used_warnings,
    include_directories : islands_private_includes,
    dependencies : [islands_dep, catch2_dep]
)


test('islands_ut', islands_test_exe)

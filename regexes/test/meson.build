regexes_ut_sources = [
    'matcher.cpp',
    'to_intermediate.cpp',
    'tests.cpp'
]

regexes_test_exe = executable(
    'regexes_ut',
    regexes_ut_sources,
    cpp_args : used_warnings,
    include_directories : regexes_private_includes,
    dependencies : [regexes_dep, catch2_dep]
)


test('regexes_ut', regexes_test_exe)

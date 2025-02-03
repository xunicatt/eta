project(
  'eta',
  'cpp',
  default_options: ['cpp_std=c++23', 'cpp_flags=-Wall -Werror -Wextra'],
)

# dirs
subdir('src/debug')
subdir('src/token')
subdir('src/types')
subdir('src/lexer')
subdir('src/ast')
subdir('src/parser')
subdir('src/object')
subdir('src/evaluator')
subdir('src/repl')

executable(
  'eta',
  'main.cpp',
  dependencies: [
    debug_dep,
    token_dep,
    types_dep,
    lexer_dep,
    ast_dep,
    parser_dep,
    object_dep,
    evaluator_dep,
    repl_dep,
  ],
)

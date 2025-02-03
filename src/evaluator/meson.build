# user config
name = 'evaluator'
srcs = [
  'evaluator.cpp',
  'errors.cpp',
  'basics.cpp',
  'boolean.cpp',
  'controls.cpp',
  'expressions.cpp',
  'functions.cpp',
  'builtins.cpp',
]

# presets
set_variable(
  name + '_lib',
  static_library(
    name,
    srcs,
    dependencies: [
      debug_dep,
      token_dep,
      types_dep,
      lexer_dep,
      ast_dep,
      parser_dep,
      object_dep,
    ],
  ),
)
set_variable(
  name + '_dep',
  declare_dependency(
    link_with: get_variable(name + '_lib'),
    include_directories: include_directories('.'),
  ),
)

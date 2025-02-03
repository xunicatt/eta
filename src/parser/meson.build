# user config
name = 'parser'
srcs = [
  'parser.cpp',
  'basics.cpp',
  'controls.cpp',
  'errors.cpp',
  'expressions.cpp',
  'functions.cpp',
  'primitives.cpp',
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

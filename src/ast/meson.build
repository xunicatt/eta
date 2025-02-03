# user config
name = 'ast'
srcs = [
  'ast.cpp',
  'expressions.cpp',
  'primitives.cpp',
  'statement.cpp',
]

# presets
set_variable(
  name + '_lib',
  static_library(
    name,
    srcs,
    dependencies: [
      token_dep,
      types_dep,
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

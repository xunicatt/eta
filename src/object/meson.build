# user config
name = 'object'
srcs = [
  'primitives.cpp',
  'environments.cpp',
  'primitives.cpp',
  'specials.cpp',
]

# presets
set_variable(
  name + '_lib',
  static_library(
    name,
    srcs,
    dependencies: [
      types_dep,
      token_dep,
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

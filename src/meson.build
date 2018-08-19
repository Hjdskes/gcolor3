headers = [
  'gcolor3-application.h',
  'gcolor3-color-row.h',
  'gcolor3-color-selection.h',
  'gcolor3-color-store.h',
  'gcolor3-hsv.h',
  'gcolor3-window.h',
  'utils.h'
]

sources = [
  'gcolor3-application.c',
  'gcolor3-color-row.c',
  'gcolor3-color-selection.c',
  'gcolor3-color-store.c',
  'gcolor3-hsv.c',
  'gcolor3-window.c',
  'main.c',
  'utils.c'
]

sources += gnome.compile_resources(
  'gcolor3-resources', join_paths(meson.source_root(), 'data', 'gcolor3.gresource.xml'),
  source_dir: join_paths(meson.source_root(), 'data'),
  c_name: 'gcolor3'
)

sources += gnome.genmarshal(
  'gcolor3-marshalers',
  sources: join_paths(meson.source_root(), 'data', 'gcolor3-marshalers.list'),
  prefix: '_gcolor3_marshal'
)

executable(
  meson.project_name(),
  sources + headers,
  include_directories: [
    include_directories('../data', '..')
  ],
  dependencies: [
    dep_gtk,
    dep_lm
  ],
  install: true,
  install_dir: bindir
)

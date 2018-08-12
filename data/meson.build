install_data('nl.hjdskes.gcolor3.svg',
  install_dir: join_paths(datadir, 'icons', 'hicolor', 'scalable', 'apps'))

i18n.merge_file(
  input: 'nl.hjdskes.gcolor3.desktop.in',
  output: 'nl.hjdskes.gcolor3.desktop',
  type: 'desktop',
  po_dir: join_paths(meson.source_root(), 'po'),
  install: true,
  install_dir: join_paths(datadir, 'applications')
)

i18n.merge_file(
  input: 'nl.hjdskes.gcolor3.appdata.xml.in',
  output: 'nl.hjdskes.gcolor3.appdata.xml',
  type: 'xml',
  po_dir: join_paths(meson.source_root(), 'po'),
  install: true,
  install_dir: join_paths(datadir, 'metainfo')
)

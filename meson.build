project('gcolor3', 'c', version: '2.3.1',
  license: 'GPL2+',
  meson_version: '>= 0.40.0',
  default_options: [
    'c_std=c99',
    'werror=true'
  ]
)

dep_gtk = dependency('gtk+-3.0', version: '>= 3.20.0', required: true)
dep_libportal = dependency('libportal', required: true)
cc = meson.get_compiler('c')
dep_lm = cc.find_library('m', required: true)

i18n= import('i18n')
gnome = import('gnome')

prefix = get_option('prefix')
bindir = join_paths(prefix, get_option('bindir'))
datadir = join_paths(prefix, get_option('datadir'))
localedir = join_paths(prefix, get_option('localedir'))

subdir('data')
subdir('src')
subdir('po')

defines = [
  ['PACKAGE', meson.project_name()],
  ['PACKAGE_NAME', meson.project_name()],
  ['PACKAGE_URL', 'https://hjdskes.nl/projects/gcolor3/'],
  ['PACKAGE_VERSION', meson.project_version()],
  ['GETTEXT_PACKAGE', meson.project_name()],
  ['LOCALE_DIR', localedir],
  ['COPYRIGHT', '2013 - 2018'],
]

config_h = configuration_data()
foreach define: defines
  config_h.set_quoted(define[0], define[1])
endforeach

config_h.set('UNUSED', '__attribute__ ((unused))')

configure_file(output: 'config.h',
  configuration: config_h)

meson.add_install_script('meson_install.sh', get_option('datadir'))

output = '\n        ' + meson.project_name() + ' ' + meson.project_version() + '\n'
output += '        ==================\n\n'
output += '        prefix:        ' + prefix + '\n'
output += '        bindir:        ' + bindir + '\n'
output += '        datadir:       ' + datadir + '\n'
output += '        localedir:     ' + localedir + '\n'
message(output)


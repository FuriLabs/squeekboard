project(
    'squeekboard',
    'c', 'rust',
    version: '1.39.0',
    license: 'GPLv3',
    meson_version: '>=0.51.0',
    default_options: [
        'warning_level=1',
        'buildtype=debugoptimized',
        'c_std=gnu11'
    ]
)

add_project_arguments(
  [
    '-Werror=implicit-function-declaration',
    '-Werror=implicit-fallthrough=3',
    '-Werror=maybe-uninitialized',
    '-Werror=missing-field-initializers',
    '-Werror=incompatible-pointer-types',
    '-Werror=int-conversion',
    '-Werror=redundant-decls',
    '-Werror=parentheses',
    '-Wformat-nonliteral',
    '-Wformat-security',
    '-Wformat',
    '-Winit-self',
    '-Wmaybe-uninitialized',
    '-Wold-style-definition',
    '-Wredundant-decls',
    '-Wstrict-prototypes',
    '-Wunused',
  ],
  language: 'c'
)

i18n = import('i18n')

if get_option('buildtype').startswith('debug')
    add_project_arguments('-DDEBUG=1', language : 'c')
endif

if get_option('strict')
    add_project_arguments(
        [
            '-Werror',
        ],
        language: 'c'
    )
endif

if get_option('buildtype') != 'plain'
    add_project_arguments('-fstack-protector-strong', language: 'c')
endif
if get_option('buildtype') == 'release'
    cargo_build_flags = ['--release'] # for artifacts
else
    cargo_build_flags = []
endif

prefix = get_option('prefix')
bindir = join_paths(prefix, get_option('bindir'))
datadir = join_paths(prefix, get_option('datadir'))
localedir = join_paths(prefix, get_option('localedir'))
desktopdir = join_paths(datadir, 'applications')
pkgdatadir = join_paths(datadir, meson.project_name())
if get_option('depdatadir') == ''
  depdatadir = datadir
else
  depdatadir = get_option('depdatadir')
endif
dbusdir = join_paths(depdatadir, 'dbus-1/interfaces')

conf_data = configuration_data()
conf_data.set_quoted('GETTEXT_PACKAGE', 'squeekboard')
conf_data.set_quoted('LOCALEDIR', localedir)

summary = [
        '',
        '------------------',
        'squeekboard @0@'.format(meson.project_version()),
        '',
        '------------------',
        ''
]
message('\n'.join(summary))

path_data = configuration_data()
path_data.set('path', meson.source_root())
cargo_toml = configure_file(
    input: 'Cargo.toml',
    output: 'Cargo.toml',
    configuration: path_data,
)

cargo_build_flags += ['--features', 'zbus_v1_5,clap_v4']

dep_cargo = find_program('cargo')
cargo_script = find_program('cargo.sh')
cargo_build = find_program('cargo_build.py')

subdir('data')
subdir('po')
subdir('protocols')
subdir('src')
subdir('tools')
#subdir('tests')

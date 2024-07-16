gnome = import('gnome')

squeekboard_resources = gnome.compile_resources(
   'squeekboard-resources',
   'squeekboard.gresources.xml',

   c_name: 'squeekboard',
)

desktopconf = configuration_data()
desktopconf.set('bindir', bindir)

desktop_file = 'sm.puri.Squeekboard.desktop'

i18n.merge_file(
    input: configure_file(
      input: desktop_file + '.in.in',
      output: desktop_file + '.in',
      configuration: desktopconf
    ),
    output: desktop_file,
    po_dir: '../po',
    install: true,
    install_dir: desktopdir,
    type: 'desktop'
)

gsettings_schema = 'sm.puri.Squeekboard.gschema.xml'
gnome.compile_schemas(build_by_default: true, depend_files: gsettings_schema)
install_data(gsettings_schema, install_dir: depdatadir + '/glib-2.0/schemas')
gnome.post_install (glib_compile_schemas: true)

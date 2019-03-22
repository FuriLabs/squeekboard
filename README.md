*squeekboard* - a Wayland virtual keyboard
========================================

*Squeekboard* is a virtual keyboard supporting Wayland, built primarily for the *Librem 5* phone.

Features
--------

### Present

- GTK3
- Custom xml-defined keyboards
- DBus interface to show and hide

### Temporarily dropped

- A settings interface

### TODO

- Use Wayland virtual keyboard protocol
- Use Wayland text input protocol
- Use Wayland input method protocol
- Become a Wayland layer shell
- Remove eekboard-client
- Use sm.puri.OSK0 DBus interface
- Pick up DBus interface files from /usr/share

Building
--------

### Dependencies

See `.gitlab-ci.yml`.

### Build from git repo

```
$ git clone https://source.puri.sm/Librem5/eekboard.git
$ cd eekboard
$ mkdir ../build
$ meson ../build/
$ cd ../build
$ ninja install
```

Running
-------

```
$ rootston
$ cd ../build/
$ src/squeekboard
$ busctl call --user org.fedorahosted.Eekboard /org/fedorahosted/Eekboard org.fedorahosted.Eekboard ShowKeyboard
$ busctl call --user org.fedorahosted.Eekboard /org/fedorahosted/Eekboard org.fedorahosted.Eekboard HideKeyboard
```
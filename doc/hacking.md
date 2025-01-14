Hacking
=======

This document describes the standards for modifying and maintaining the *squeekboard* project.

Principles
----------

The project was built upon some guiding principles, which should be respected primarily by the maintainers, but also by contributors to avoid needlessly rejected changes.

The overarching principle of *squeekboard* is to empower users.

Software is primarily meant to solve problems of its users. Often in the quest to make software better, a hard distinction is made between the developer, who becomes the creator, and the user, who takes the role of the consumer, without direct influence on the software they use.
This project aims to give users the power to make the software work for them by blurring the lines between users and developers.

Notwithstanding its current state, *squeekboard* must be structured in a way that provides users a gradual way to gain more experience and power to adjust it. It must be easy, in order of importance:

- to use the software,
- to modify its resources,
- to change its behavior,
- to contribute upstream.

To give an idea of what it means in practice, those are some examples of what has been important for *squeekboard* so far:

- being quick and usable,
- allowing local overrides of resources and config,
- storing resources and config as editable, standard files,
- having complete, up to date documentation of interfaces,
- having an easy process of sending contributions,
- adapting to the user's settings and constraints without overriding them,
- avoiding compiling whenever possible,
- making it easy to build,
- having code that is [simple and obvious](https://www.python.org/dev/peps/pep-0020/),
- having an easy process of testing and accepting contributions.

You may notice that they are ordered roughly from "user-focused" to "maintainer-focused". While good properties are desired, sometimes they conflict, and maintainers should give additional weight to those benefiting the user compared to those benefiting regular contributors.

Sending patches
---------------

By submitting a change to this project, you agree to license it under the [GPL license version 3](COPYING), or any later version. You also certify that your contribution fulfills the [Developer's Certificate of Origin 1.1](dco.txt).

Development environment
-----------------------

*Squeekboard* is regularly built and tested on [Debian Testing](https://www.debian.org/releases/testing/) and [Mobian Testing](https://mobian.org/).

For testing Squeekboard on a PC as if it was used on a smartphone, one can use an emulator. Instructions for that can be found in the [documentation for setting up a development-environment for the Librem 5](https://developer.puri.sm/Librem5/Development_Environment/Boards/emulators.html).

### Dependencies

On a Debian based system run

```sh
$ sudo apt-get -y install build-essential
$ sudo apt-get -y build-dep .
```

For an explicit list of dependencies check the `Build-Depends` entry in the [`debian/control`](debian/control) file.

Note: Some build-dependencies are currently unavailable in Debian Testing.
To be able to install the build-dependencies with the previously mentioned commands anyway,
rename `control` in the `debian`-folder to `control.original`,
and `control.debian-testing` to `control` before using those commands.
The remaining dependencies will then be downloaded by `cargo` when building Squeekboard.

Testing
-------

Most common testing is done in CI. Occasionally, and for each release, do perform manual tests to make sure that

- the application draws correctly
- it shows when relevant
- it changes layouts
- it changes views

#### Testing with an application

```sh
$ python3 tools/entry.py
```

#### Testing visibility

```sh
$ busctl call --user sm.puri.OSK0 /sm/puri/OSK0 sm.puri.OSK0 SetVisible b true
$ busctl call --user sm.puri.OSK0 /sm/puri/OSK0 sm.puri.OSK0 SetVisible b false
```

#### Testing layouts

Available Layouts can be selected by using the GNOME Settings application.

Those can also be set with `gsettings`:

```sh
# define all available layouts. First one is currently selected.
$ gsettings set org.gnome.desktop.input-sources sources "[('xkb', 'us'), ('xkb', 'de')]"
```

### Debugging mode

Squeekboard prints some information on standard output by default. To get deep debugging information, it can also print all changes in (some of) its internal state. Those logs are most useful when reporting hard to catch issues, and can be enabled using the following command:

```sh
$ busctl set-property --user sm.puri.SqueekDebug /sm/puri/SqueekDebug sm.puri.SqueekDebug Enabled b true
```

### Environment Variables

Besides the environment variables supported by GTK and [GLib](https://docs.gtk.org/glib/running.html) applications
squeekboard honors the `SQUEEKBOARD_DEBUG` environment variable which can
contain a comma separated list of:

- `force-show` : Show squeekboard on startup independent of any gsettings or compositor requests
- `gtk-inspector`: Spawn [gtk-inspector](https://wiki.gnome.org/Projects/GTK/Inspector)


`GTK_THEME=` can be used to choose a theme other than the default theme for Squeekboard:

  - `Adwaita:dark` is used for Squeekboard on Phosh.
  - Other values that are not the name of an available theme (for example: `HighContrast`) will use the theme that is used while "High Contrast" is enabled in Phosh.

Coding
------

### Reference docs

Reference documentation can be generated using:

```sh
$ cd squeekboard_build/
$ .../squeekboard_source/cargo.sh doc --no-deps --document-private-items
```

as well as found [online](https://world.pages.gitlab.gnome.org/Phosh/squeekboard/doc/rs/).

### Project structure

Rust modules should be split into 2 categories: libraries, and user interface. They differ in the way they do error handling.

Libraries should:

- not panic due to external surprises, only due to internal inconsistencies
- pass errors and surprises they can't handle to the callers instead
- not silence errors and surprises

User interface modules should:

- try to provide safe values whenever they encounter an error
- do the logging
- give libraries the ability to report errors and surprises (e.g. via giving them loggers)

### Style

Note that some portions, like the .gitlab-ci.yml file have accummulated enough style/whitespace conflicts that an enforced style checker is now applied.

To fix your contributions before submitting a change, use:

```sh
$ ./tools/style-check_source --apply
```

* * *

Code submitted should roughly match the style of surrounding code. Things that will *not* be accepted are ones that often lead to errors:

- skipping brackets `{}` after every `if()`, `else`, and similar ([SCI CERT C: EXP19-C](https://wiki.sei.cmu.edu/confluence/display/c/EXP19-C.+Use+braces+for+the+body+of+an+if%2C+for%2C+or+while+statement))

Bad example:

```
if (foo)
  bar();
```

Good example:

```
if (foo) {
  bar();
}
```

- mixing tabs and spaces in the same block of code (or config)

Strongly encouraged:

- don't make lines too long. If it's longer than ~80 characters, it's probably unreadable already, and the code needs to be clarified;
- put operators in the beginning of a continuation line

Bad example:

```
foobar = verylongexpression +
    anotherverylongexpression + 
    yetanotherexpression;
```

Good example:

```
foobar = verylongexpression
    + anotherverylongexpression
    + yetanotherexpression;
```

- use `///` for documentation comments in front of definitions and `/*! ... */` for documentation comments in the beginning of modules (see [Rust doc-comments](https://doc.rust-lang.org/reference/comments.html#doc-comments))

If in doubt, check [PEP8](https://github.com/rust-dev-tools/fmt-rfcs/blob/master/guide/guide.md), the [kernel coding style](https://www.kernel.org/doc/html/v4.10/process/coding-style.html), or the [Rust style guide](https://github.com/rust-dev-tools/fmt-rfcs/blob/master/guide/guide.md).

Maintenance
-----------

Squeekboard uses Rust & Cargo for some of its dependencies.

Use the `cargo.sh` script for maintaining the Cargo part of the build. The script takes the usual Cargo commands, after the first 2 positional arguments: source directory, and output artifact. So, `cargo test` becomes:

```sh
$ cd build_dir
$ sh /source_path/cargo.sh test
```

### Cargo dependencies

All Cargo dependencies must be selected in the version available in Debian Testing, and added to the file `debian/control`. Please check with the [Debian package search](https://www.debian.org/distrib/packages).

Dependencies must be specified in `Cargo.toml` with 2 numbers: "major.minor". Since bugfix version number is meant to not affect the interface, this allows for safe updates.

Releases
--------

Feature-releases should me made in time for new [Phosh releases](https://gitlab.gnome.org/World/Phosh/phosh/-/wikis/Releases) (which is currently about once a month), so that the release-notes can contain the news about Squeekboard.
However, it is not necessary to make a new release of Squeekboard for every release of Phosh.

Bug-fix-releases should be made more often, preferably directly after important bug-fixes have been made.

### 1. Update `Cargo.lock`

While the file is not actually used, it's a good idea to save the config in case some rare bug appears in dependencies.

```sh
$ cd squeekboard-build
$ .../squeekboard-source/cargo.sh update
$ ninja test
$ cp ./Cargo.lock .../squeekboard-source
```

Then commit the updated `Cargo.lock`.

### 2. Choose the version number

Squeekboard follows [Phosh's versioning](https://gitlab.gnome.org/World/Phosh/phosh/-/wikis/Releases).
For example: The first Squeekboard-release for Phosh 0.38 should have the version-number 1.38.0. The last part of the version number (1.38.x) may be incremented independently of Phosh's version for bug-fix-releases.

### 3. Update the number in `meson.build`

It's in the `project(version: xxx)` statement.

### 4. Update packaging

Packaging is in the `debian/` directory, and creates builds that can be quickly tested.

```sh
$ cd squeekboard-source
$ EMAIL=my_address@example.com gbp dch --multimaint-merge --ignore-branch --git-author --distribution=experimental --new-version=x.y.z
```

Inspect `debian/changelog`, and make sure the first line contains the correct version number and suite. For example:

```
squeekboard (1.22.0) experimental; urgency=medium
```

Add the updated `debian/changelog` to the commit. The commit message should contain the release version and a description of changes.

(`gbp` can be installed on Debian based systems with `sudo apt install git-buildpackage`)

### 5. Update the NEWS file

Summarize the changes since the last release in the NEWS file. Use the Markdown syntax, e.g.

```
1.22.0 "Superposition"
-----------------------------

Changes:
- Fixed panel sizing when scaling
...
```

### 6. Commit changes

Generate a commit message from the NEWS file:

```sh
$ ./tools/make_message | git commit --file=- ...
```

If the commit message looks wrong, fix the NEWS file, and do `git commit --amend`.

### 7. Create a signed tag for downstreams

The tag should be the version number with "v" in front of it. The tag message should be "squeekboard" and the tag name. Push it to the upstream repository:

```sh
$ git tag -s -u my_address@example.com v1.22.0 -m "squeekboard v1.22.0"
$ git push origin v1.22.0
```

### 8. Rejoice

You released a new version of Squeekboard. Congratulations!

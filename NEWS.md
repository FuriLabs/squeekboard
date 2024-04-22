1.38.0
------------------

Changes:
- 25 wide shapes have been added, so that every available layout now has a wide shape
- "PgUp" and "PgDn" on the terminal-layouts have been relabeled to "Page ↑" and "Page ↓"
- The Spanish and French terminal-layouts now have translated key-names
- The Spanish terminal-layout has been updated with the additional keys that are already available on the US-terminal-layout.
- The wide and base shapes of the German layout had a different key-arrangement and the wide shape did not have a button to access additional characters; this has been fixed.

Development:
- Squeekboard's versioning now follows Phosh's versioning (for example: Squeekboard 1.38 was released in time for Phosh 0.38)
- The build-system has been simplified
  - A single Cargo.toml file is used, instead of assembling it from multiple parts
  - Newer dependencies are now used for building Squeekboard by default
- Squeekboard's main development-platform is now Debian Testing
- The layout-files have been cleaned up, so that those are easier to understand and edit

1.24.0
------------------

Changes:
- The emoji-layout has been replaced with a new one, which offers many more emojis to choose from.

1.23.0
------------------

New or updated translations:
- Belarusian
- Haitian Creole

New layouts:
- French Canadian (QWERTY + accented letters)
- German terminal-layout
- Spanish terminal-layout

Changes:
- Fixed Persian and Swiss layouts
- Fixed various small style-issues in many layouts
- Improved the US-terminal-layout

1.22.0 "Superposition"
------------------

New or updated translations:
- Basque

Changes:
- fixed panel sizing when scaling
- fixed panel sizing when rotating
- fixed Dvorak terminal layout

1.21.0 "Expected value"
------------------

New or updated translations:
- Hindi
- Czech
- German

New layouts:
- wide Swedish
- Hungarian

Changes:
- use a custom font for gr+polytonic, where the default is unreadable
- require newer Rust
- fixed panel sizing when rotating
- internal improvements.

1.20.0 "PID controller"
------------------

New translations:
- Greek
- Croatian

New layouts:
- US Dvorak terminal

Improvements:
- forcing the panel to hide now takes effect immediately
- Squeekboard icon will present itself when other applications need to show it

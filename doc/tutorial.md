Kareema's guide to creating layouts
===================================

It’s long overdue to write a comprehensive guide how to add a keyboard layout from start. But unfortunately, I don’t have much time left ATM. A lot of information can be found in [this ](https://forums.puri.sm/t/using-non-latin-language-on-librem-5/7103/5) thread.

So at least I will try to start writing a short how-to here and edit this post as I find the time. Hope this helps a bit - comments and corrections welcome.

**Get one of the existing keyboard layouts**

* You can get one of the keyboards from the squeekboard git repository : [https://source.puri.sm/Librem5/squeekboard ](https://source.puri.sm/Librem5/squeekboard)
* The keyboard layouts are located in the subdirectory `data/keyboard/` in the `.yaml` files
* Take a look and try to understand them :slight_smile:

**Fork your own copy of squeekboard**

* Best way would be to start with a fork of the squeekboard repository: Create a user account at https://source.puri.sm/, go the the squeekboard git repository, press “Fork” in the web interface. You can find further instructions [here](https://docs.gitlab.com/ee/user/project/repository/forking_workflow.html#creating-a-fork).
* Clone your fork locally with `git clone` and use the uri of your forked repo there

**Workflow to edit your keyboard and get it merged**

* A generic guide how the workflow to contribute works, can be found at https://developer.puri.sm/Librem5/Contact/Contributing.html
* Create a branch: Name it “keyboard-layout-mylanguage” or whatever
* Checkout your branch, edit your keyboard layout and commit your changes
* Push the local changes (to the branch of your fork of squeekboard)
* Create a merge request for the branch to get your changes merged to the official squeekboard git repository

**Compile squeekboard**

* Follow the instructions found in “Building” section of the squeekboard’s README: Running squeekboard: [https://source.puri.sm/Librem5/squeekboard/blob/master/README.md#building ](https://source.puri.sm/Librem5/squeekboard/blob/master/README.md#building)

**Running squeekboard**

* Follow these instructions to run squeekboard: [https://source.puri.sm/Librem5/squeekboard/blob/master/README.md#running ](https://source.puri.sm/Librem5/squeekboard/blob/master/README.md#running)
* Additionally take a look at the contribution document for [testing info](HACKING.md#testing)
* You can either test it locally on your Linux system or use the [QEMU Librem 5 image ](https://developer.puri.sm/Librem5/Development_Environment/Boards/emulators.html)
* To test squeekboard locally, you need phoc. Either compile that from the sources as well or use the CI repository ci.puri.sm for Debian based systems:
  `deb [arch=amd64] http://ci.puri.sm/ scratch librem5`

Squeekboard can be installed from there as a Debian package, too (that’s what I often do). But beware - there be dragons! You could bork your system with these packages and you should probably disable this repository again after installing what you need - these packages are not meant for production systems (or so I heard :wink: )

**Creating the keyboard layout**

* To be written: For the time being, take a look at [Using non-latin language on Librem 5 ](https://forums.puri.sm/t/using-non-latin-language-on-librem-5/7103/5)
* The correct name of the .yaml file can be found with the command `gsettings get org.gnome.desktop.input-sources sources`

The output should be something like this: `[('xkb', 'us'), ('xkb', 'de')]`
So f.ex. “de.yaml” would be the correct name for the German keyboard layout.
* The translations for the keyboard layout names in the different languages can be found at `data/langs/`
* Don’t forget to add your newly created layout or translation to `src/resources.rs` and the layout to `tests/meson.build` (that’s for me, because I always forget it)

**Testing the layout**

* Copy your yaml file to `~/.local/share/squeekboard/keyboards/` for testing purposes. From there it should get picked up by squeekboard
* To test the translations in `data/langs/` , you have to compile squeekboard


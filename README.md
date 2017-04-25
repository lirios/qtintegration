Qt Platform Theme plugin
========================

[![ZenHub.io](https://img.shields.io/badge/supercharged%20by-zenhub.io-blue.svg)](https://zenhub.io)

[![License](https://img.shields.io/badge/license-GPLv3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![GitHub release](https://img.shields.io/github/release/lirios/platformtheme.svg)](https://github.com/lirios/platformtheme)
[![Build Status](https://travis-ci.org/lirios/fluid.svg?branch=develop)](https://travis-ci.org/lirios/platformtheme)
[![GitHub issues](https://img.shields.io/github/issues/lirios/platformtheme.svg)](https://github.com/lirios/platformtheme/issues)
[![Maintained](https://img.shields.io/maintenance/yes/2017.svg)](https://github.com/lirios/platformtheme/commits/develop)

Qt platform theme plugin for apps integration with Liri OS.

## Dependencies

Qt >= 5.8.0 with at least the following modules is required:

 * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
 * [qtquickcontrols2](http://code.qt.io/cgit/qt/qtquickcontrols2.git)

And the following modules:

 * [qtgsettings](https://github.com/lirios/qtgsettings.git)

## Installation

From the root of the repository, run:

```sh
mkdir build; cd build
qmake ../platformtheme.pro
make
make install # use sudo if necessary
```

On the `qmake` line, you can specify additional configuration parameters:

 * `LIRI_INSTALL_PREFIX=/path/to/install` (for example `/opt/liri` or `/usr`)
 * `CONFIG+=debug` if you want a debug build
 * `CONFIG+=use_qt_paths` as an alternative to `LIRI_INSTALL_PREFIX=/path/to/install`
   if you want to install where Qt is installed

Use `make distclean` from inside your `build` directory to clean up.
You need to do this before rerunning `qmake` with different options.

### Notes

A system-wide installation with `LIRI_INSTALL_PREFIX=/usr` is usually performed
by Linux distro packages.

In order to avoid potential conflicts we recommend installing under `/opt/liri`,
but this requires setting some environment variables up.

First build and install:

```sh
mkdir build; cd build
qmake LIRI_INSTALL_PREFIX=/opt/liri ..
make
sudo make install
```

Then create a file with the environment variables as `~/lenv` with the following contents:

```sh
LIRIDIR=/opt/liri

export LD_LIBRARY_PATH=$LIRIDIR/lib:$LD_LIBRARY_PATH
export XDG_DATA_DIRS=$LIRIDIR/share:/usr/local/share:/usr/share:~/.local/share:~/.local/share/flatpak/exports/share
export XDG_CONFIG_DIRS=$LIRIDIR/etc/xdg:/etc/xdg
export QT_PLUGIN_PATH=$LIRIDIR/lib/plugins
export QML2_IMPORT_PATH=$LIRIDIR/lib/qml:$QML2_IMPORT_PATH
export PATH=$LIRIDIR/bin:$PATH
```

Source the file (we are assuming a bash shell here):

```sh
source ~/lenv
```

And run an application to test if it picks up Liri settings:

```sh
qdbusviewer -platformtheme liri
```

## Licensing

Licensed under the terms of the GNU General Public License version 3.0.

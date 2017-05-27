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

### Build with Qbs

Qbs is a new build system that is much easier to use compared to qmake or CMake.
It is the default build system for this project and soon will become the only one.

If you want to learn more, please read the [Qbs manual](http://doc.qt.io/qbs/index.html),
especially the [setup guide](http://doc.qt.io/qbs/configuring.html) and how to install artifacts
from the [installation guide](http://doc.qt.io/qbs/installing-files.html).

From the root of the repository, run:

```sh
qbs setup-toolchains --type gcc /usr/bin/g++ gcc
qbs setup-qt /usr/bin/qmake-qt5 qt5
qbs config profiles.qt5.baseProfile gcc
qbs -d build -j $(nproc) profile:qt5 # use sudo if necessary
```

On the last `qbs` line, you can specify additional configuration parameters at the end:

 * `qbs.installRoot:/path/to/install` (for example `/`)
 * `qbs.installPrefix:path/to/install` (for example `opt/liri` or `usr`)

The following are only needed if `qbs.installPrefix` is a system-wide path such as `usr`
and the default value doesn't suit your needs. All are relative to `qbs.installRoot`:

 * `lirideployment.libDir:path/to/lib` where libraries are installed (default: `lib`)
 * `lirideployment.qmlDir:path/to/qml` where QML plugins are installed (default: `lib/qml`)
 * `lirideployment.pluginsDir:path/to/plugins` where Qt plugins are installed (default: `lib/plugins`)
 * `lirideployment.qbsModulesDir:path/to/qbs` where Qbs modules are installed (default: `share/qbs/modules`)

See `qbs/shared/modules/lirideployment/lirideployment.qbs` for more deployment-related parameters.

If you specify `qbs.installRoot` you might need to prefix the entire line with `sudo`,
depending on whether you have permissions to write there or not.

### Build with qmake

qmake, the Qt build system is still supported but will be removed at some point.
Qbs is a much better alternative with none of the complexity brought by qmake.

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

#### Notes

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

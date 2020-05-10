Qt Platform Theme plugin
========================

[![License](https://img.shields.io/badge/license-GPLv3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![GitHub release](https://img.shields.io/github/release/lirios/platformtheme.svg)](https://github.com/lirios/platformtheme)
[![GitHub issues](https://img.shields.io/github/issues/lirios/platformtheme.svg)](https://github.com/lirios/platformtheme/issues)
[![CI](https://github.com/lirios/platformtheme/workflows/CI/badge.svg?branch=develop)](https://github.com/lirios/platformtheme/actions?query=workflow%3ACI)

Qt platform theme plugin for apps integration with Liri desktop.

This plugin extends Qt hence you have to rebuild it every time Qt is updated.

## Dependencies

Qt >= 5.8.0 with at least the following modules is required:

 * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
 * [qtquickcontrols2](http://code.qt.io/cgit/qt/qtquickcontrols2.git)

And the following modules:

 * [cmake](https://gitlab.kitware.com/cmake/cmake) >= 3.10.0
 * [cmake-shared](https://github.com/lirios/cmake-shared.git) >= 1.0.0
 * [qtgsettings](https://github.com/lirios/qtgsettings.git) >= 1.1.0

## Installation

```sh
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/path/to/prefix ..
make
make install # use sudo if necessary
```

Replace `/path/to/prefix` to your installation prefix.
Default is `/usr/local`.

## Licensing

Licensed under the terms of the GNU General Public License version 3.0.

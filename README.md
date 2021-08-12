Qt applications integration
===========================

[![License](https://img.shields.io/badge/license-GPLv3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![GitHub release](https://img.shields.io/github/release/lirios/platformtheme.svg)](https://github.com/lirios/platformtheme)
[![GitHub issues](https://img.shields.io/github/issues/lirios/platformtheme.svg)](https://github.com/lirios/platformtheme/issues)
[![CI](https://github.com/lirios/platformtheme/workflows/CI/badge.svg?branch=develop)](https://github.com/lirios/platformtheme/actions?query=workflow%3ACI)

Qt applications integration with the Liri desktop environment.

The plugins included in this repository use Qt private APIs,
hence you have to rebuild it every time Qt is updated.

Contains:

  * Platform theme plugin.
  * Client-side decoration for Qt applications on Wayland.
  * Wayland shell integration for layer-shell and xdg-shell.

**Material Decoration**

Set `QT_WAYLAND_DECORATION=material` in the environment to see Qt applications
using Material Design window decorations in a Wayland session.

Make sure `QT_WAYLAND_DISABLE_WINDOWDECORATION` is not set, otherwise Qt will
not use any client-side decoration.

## Dependencies

Qt >= 5.15.0 with at least the following modules is required:

 * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
 * [qtquickcontrols2](http://code.qt.io/cgit/qt/qtquickcontrols2.git)
 * [qtwayland](http://code.qt.io/cgit/qt/qtwayland.git)

And the following modules:

 * [cmake](https://gitlab.kitware.com/cmake/cmake) >= 3.10.0
 * [cmake-shared](https://github.com/lirios/cmake-shared.git) >= 1.0.0
 * [qtgsettings](https://github.com/lirios/qtgsettings.git) >= 1.1.0
 * [liri-wayland](https://github.com/lirios/wayland)
 * [wayland-protocols](https://gitlab.freedesktop.org/wayland/wayland-protocols) >= 1.15
 * [xkbcommon](https://github.com/xkbcommon/libxkbcommon)

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

Licensed under the terms of the GNU Lesser General Public License version 3 or,
at your option, any later version.

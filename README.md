Qt Platform Theme plugin
========================

[![ZenHub.io](https://img.shields.io/badge/supercharged%20by-zenhub.io-blue.svg)](https://zenhub.io)

[![License](https://img.shields.io/badge/license-GPLv3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![GitHub release](https://img.shields.io/github/release/lirios/platformtheme.svg)](https://github.com/lirios/platformtheme)
[![Build Status](https://travis-ci.org/lirios/fluid.svg?branch=develop)](https://travis-ci.org/lirios/platformtheme)
[![GitHub issues](https://img.shields.io/github/issues/lirios/platformtheme.svg)](https://github.com/lirios/platformtheme/issues)
[![Maintained](https://img.shields.io/maintenance/yes/2018.svg)](https://github.com/lirios/platformtheme/commits/develop)

Qt platform theme plugin for apps integration with Liri OS.

## Dependencies

Qt >= 5.8.0 with at least the following modules is required:

 * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
 * [qtquickcontrols2](http://code.qt.io/cgit/qt/qtquickcontrols2.git)

And the following modules:

 * [qbs-shared](https://github.com/lirios/qbs-shared.git)
 * [qtgsettings](https://github.com/lirios/qtgsettings.git)

## Installation

Qbs is a new build system that is much easier to use compared to qmake or CMake.

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

 * `modules.lirideployment.pluginsDir:path/to/plugins` where Qt plugins are installed (default: `lib/plugins`)

See `qbs/shared/modules/lirideployment/lirideployment.qbs` for more deployment-related parameters.

If you specify `qbs.installRoot` you might need to prefix the entire line with `sudo`,
depending on whether you have permissions to write there or not.

## Licensing

Licensed under the terms of the GNU General Public License version 3.0.

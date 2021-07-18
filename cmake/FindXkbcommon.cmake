# SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause

find_package(PkgConfig)

pkg_check_modules(Xkbcommon xkbcommon IMPORTED_TARGET)

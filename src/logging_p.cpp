// Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "logging_p.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
Q_LOGGING_CATEGORY(gLcPlatformTheme, "liri.platformtheme", QtInfoMsg)
#else
Q_LOGGING_CATEGORY(gLcPlatformTheme, "liri.platformtheme")
#endif

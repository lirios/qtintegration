// Copyright (C) 2015 The Qt Company Ltd.
// Copyright (C) 2015-2016 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QtGui/QPalette>
#include <QtGui/qpa/qplatformtheme.h>

class ResourceHelper
{
public:
    explicit ResourceHelper();
    ~ResourceHelper() { clear(); }

    void clear();

    static void readPalette(const QString &fileName, QPalette *pal);
    static bool readColor(QPalette *pal, QPalette::ColorRole role, const QVariant &value);

    QPalette *palettes[QPlatformTheme::NPalettes];
    QFont *fonts[QPlatformTheme::NFonts];
};

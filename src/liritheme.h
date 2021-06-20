// Copyright (C) 2010-2016 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QtCore/QHash>
#include <QtCore/QVariant>
#include <QtGui/qpa/qplatformtheme.h>

class HintsSettings;

class Q_GUI_EXPORT LiriTheme : public QPlatformTheme
{
public:
    explicit LiriTheme();

    bool usePlatformNativeDialog(DialogType type) const;
    QPlatformDialogHelper *createPlatformDialogHelper(DialogType type) const;

    const QPalette *palette(Palette type = SystemPalette) const;
    const QFont *font(Font type = SystemFont) const;

    QVariant themeHint(ThemeHint hint) const;

private:
    HintsSettings *m_hints;
};

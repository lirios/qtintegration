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

    bool usePlatformNativeDialog(DialogType type) const override;
    QPlatformDialogHelper *createPlatformDialogHelper(DialogType type) const override;

    const QPalette *palette(Palette type = SystemPalette) const override;
    const QFont *font(Font type = SystemFont) const override;

    QVariant themeHint(ThemeHint hint) const override;

private:
    HintsSettings *m_hints;
};

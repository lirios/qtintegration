// Copyright (C) 2010-2016 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QtCore/QObject>

#include "liritheme.h"
#include "resourcehelper.h"

namespace QtGSettings {
class QGSettings;
}

class HintsSettings : public QObject
{
    Q_OBJECT
public:
    explicit HintsSettings(QObject *parent = nullptr);
    ~HintsSettings();

    inline QVariant themeHint(QPlatformTheme::ThemeHint hint) const
    {
        if (m_hints.contains(hint))
            return m_hints[hint];
        return QVariant();
    }

    inline QPalette *palette(QPlatformTheme::Palette type) const
    {
        return m_resources.palettes[type];
    }

    inline QFont *font(QPlatformTheme::Font type) const
    {
        return m_resources.fonts[type];
    }

    void refresh();

private:
    QtGSettings::QGSettings *m_settings;
    QHash<QPlatformTheme::ThemeHint, QVariant> m_hints;
    ResourceHelper m_resources;

    Qt::ToolButtonStyle toolButtonStyle(const QVariant &value);
    int toolBarIconSize(const QVariant &value);

    void collectHints();
    void refreshPalette();
    void refreshFonts();

    void qtSettingsChanged();
    void toolButtonStyleChanged();
    void iconChanged();
    void styleChanged();
    void paletteChanged();
    void fontChanged();
};

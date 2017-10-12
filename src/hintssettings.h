/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2010-2016 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

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

    inline QVariant themeHint(QPlatformTheme::ThemeHint hint) const {
        if (m_hints.contains(hint))
            return m_hints[hint];
        return QVariant();
    }

    inline QPalette *palette(QPlatformTheme::Palette type) const {
        return m_resources.palettes[type];
    }

    inline QFont *font(QPlatformTheme::Font type) const {
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

    void qtSettingsChanged();
    void toolButtonStyleChanged();
    void iconChanged();
    void styleChanged();
    void paletteChanged();
};

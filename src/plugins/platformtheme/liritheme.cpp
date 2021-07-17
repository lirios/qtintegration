// Copyright (C) 2010-2016 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtGui/QFont>
#include <QtQuickControls2/QQuickStyle>

#include "liritheme.h"
#include "hintssettings.h"
#include "logging_p.h"

LiriTheme::LiriTheme()
    : QPlatformTheme()
    , m_hints(new HintsSettings())
{
    qCDebug(gLcPlatformTheme, "Initializing Liri platform theme");

    m_hints->refresh();

    // Set Material style by default
    if (QQuickStyle::name().isEmpty())
        QQuickStyle::setStyle(QLatin1String("Material"));
}

bool LiriTheme::usePlatformNativeDialog(DialogType type) const
{
    Q_UNUSED(type);
    return false;
}

QPlatformDialogHelper *LiriTheme::createPlatformDialogHelper(DialogType type) const
{
    Q_UNUSED(type);
    return nullptr;
}

const QPalette *LiriTheme::palette(Palette type) const
{
    return m_hints->palette(type);
}

const QFont *LiriTheme::font(Font type) const
{
    return m_hints->font(type);
}

QVariant LiriTheme::themeHint(ThemeHint hint) const
{
    QVariant value = m_hints->themeHint(hint);
    if (value.isValid())
        return value;
    return QPlatformTheme::themeHint(hint);
}

// Copyright (C) 2010-2016 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtGui/qpa/qplatformthemeplugin.h>

#include "liritheme.h"

class LiriThemePlugin : public QPlatformThemePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPA.QPlatformThemeFactoryInterface.5.1" FILE "liritheme.json")
public:
    explicit LiriThemePlugin(QObject *parent = 0);

    QStringList keys() const;
    QPlatformTheme *create(const QString &key, const QStringList &paramList);
};

LiriThemePlugin::LiriThemePlugin(QObject *parent)
    : QPlatformThemePlugin(parent)
{
}

QStringList LiriThemePlugin::keys() const
{
    return QStringList() << QStringLiteral("liri");
}

QPlatformTheme *LiriThemePlugin::create(const QString &key, const QStringList &paramList)
{
    Q_UNUSED(paramList);

    if (key.toLower() == QStringLiteral("liri"))
        return new LiriTheme();
    return 0;
}

#include "main.moc"

// SPDX-FileCopyrightText: 2022 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtWaylandClient/private/qwaylandshellintegrationplugin_p.h>

#include "qwaylandlockscreenintegration_p.h"

namespace QtWaylandClient {

class QWaylandLockscreenIntegrationPlugin : public QWaylandShellIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QWaylandShellIntegrationFactoryInterface_iid FILE "liri-lockscreen.json")
public:
    QWaylandShellIntegration *create(const QString &key, const QStringList &paramList) override;
};

QWaylandShellIntegration *QWaylandLockscreenIntegrationPlugin::create(const QString &key, const QStringList &paramList)
{
    Q_UNUSED(paramList);

    if (key == QLatin1String("liri-lockscreen"))
        return new QWaylandLockscreenIntegration();
    return nullptr;
}

} // namespace QtWaylandClient

#include "qwaylandlockscreenintegrationplugin.moc"

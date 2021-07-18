// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtWaylandClient/private/qwaylandshellintegrationplugin_p.h>

#include "qwaylandlayershellintegration_p.h"

namespace QtWaylandClient {

class QWaylandLayerShellIntegrationPlugin : public QWaylandShellIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QWaylandShellIntegrationFactoryInterface_iid FILE "liri-layer-shell.json")
public:
    QWaylandShellIntegration *create(const QString &key, const QStringList &paramList) override;
};

QWaylandShellIntegration *QWaylandLayerShellIntegrationPlugin::create(const QString &key, const QStringList &paramList)
{
    Q_UNUSED(paramList);

    if (key == QLatin1String("liri-layer-shell"))
        return new QWaylandLayerShellIntegration();
    return nullptr;
}

} // namespace QtWaylandClient

#include "qwaylandlayershellintegrationplugin.moc"

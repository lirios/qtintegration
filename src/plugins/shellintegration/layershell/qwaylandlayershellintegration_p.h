// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef QWAYLANDLAYERSHELLINTEGRATION_P_H
#define QWAYLANDLAYERSHELLINTEGRATION_P_H

#include <QtWaylandClient/private/qwayland-wayland.h>
#include <QtWaylandClient/private/qwaylandshellintegration_p.h>

#include "qwaylandlayershell_p.h"
#include "qwaylandxdgshell_p.h"

#include <wayland-client.h>

namespace QtWaylandClient {

class Q_WAYLAND_CLIENT_EXPORT QWaylandLayerShellIntegration : public QWaylandShellIntegration
{
public:
    bool initialize(QWaylandDisplay *display) override;
    QWaylandShellSurface *createShellSurface(QWaylandWindow *window) override;

private:
    QScopedPointer<QWaylandLayerShell> m_layerShell;
    QScopedPointer<QWaylandXdgShell> m_xdgShell;
};

} // namespace QtWaylandClient

#endif // QWAYLANDLAYERSHELLINTEGRATION_P_H

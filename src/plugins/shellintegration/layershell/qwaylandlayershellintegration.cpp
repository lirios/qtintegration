// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "qwaylandlayershellintegration_p.h"
#include "qwaylandlayersurface_p.h"
#include "qwaylandxdgsurface_p.h"

namespace QtWaylandClient {

bool QWaylandLayerShellIntegration::initialize(QWaylandDisplay *display)
{
    const auto layerShellInterfaceName = QString::fromLatin1(QWaylandLayerShell::interface()->name);
    const auto xdgShellInterfaceName = QString::fromLatin1(QWaylandXdgShell::interface()->name);

    for (const QWaylandDisplay::RegistryGlobal &global : display->globals()) {
        if (global.interface == layerShellInterfaceName && !m_layerShell)
            m_layerShell.reset(new QWaylandLayerShell(display->wl_registry(), global.id, std::min(global.version, 4u)));
        else if (global.interface == xdgShellInterfaceName && !m_xdgShell)
            m_xdgShell.reset(new QWaylandXdgShell(display->wl_registry(), global.id, std::min(global.version, 3u)));
    }

    if (!m_layerShell) {
        qCWarning(lcQpaWayland) << "Couldn't find global" << layerShellInterfaceName << "for liri-layer-shell";
        return false;
    }

    if (!m_xdgShell) {
        qCWarning(lcQpaWayland) << "Couldn't find global" << xdgShellInterfaceName << "for liri-layer-shell";
        return false;
    }

    return QWaylandShellIntegration::initialize(display);
}

QWaylandShellSurface *QWaylandLayerShellIntegration::createShellSurface(QWaylandWindow *window)
{
    if (window->window()->type() == Qt::Popup)
        return new QWaylandXdgSurface(m_xdgShell.data(), window);
    else
        return new QWaylandLayerSurface(m_layerShell.data(), window);
}

} // namespace QtWaylandClient

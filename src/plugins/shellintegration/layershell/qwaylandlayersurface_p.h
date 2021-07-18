// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef QWAYLANDLAYERSURFACE_P_H
#define QWAYLANDLAYERSURFACE_P_H

#include <QtWaylandClient/qtwaylandclientglobal.h>
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>

#include <LiriWaylandClientLayerShell/LayerSurface>

#include "qwaylandlayershell_p.h"

namespace QtWaylandClient {

using namespace Liri::WaylandClient;

class Q_WAYLAND_CLIENT_EXPORT QWaylandLayerSurface
        : public QWaylandShellSurface
        , public QtWayland::zwlr_layer_surface_v1
{
    Q_OBJECT
public:
    QWaylandLayerSurface(QWaylandLayerShell *shell, QWaylandWindow *window);
    virtual ~QWaylandLayerSurface();

    bool isExposed() const override;

    void applyConfigure() override;

public Q_SLOTS:
    void setLayer(LayerSurface::Layer layer);
    void setAnchors(LayerSurface::Anchors anchors);
    void setExclusiveZone(qint32 exclusiveZone);
    void setMargins(const QMargins &margins);
    void setKeyboardInteractivity(LayerSurface::KeyboardInteractivity keyboardInteractivity);

protected:
    void zwlr_layer_surface_v1_configure(uint32_t serial, uint32_t width, uint32_t height) override;
    void zwlr_layer_surface_v1_closed() override;

private:
    QWaylandLayerShell *m_shell = nullptr;
    bool m_configured = false;
    QSize m_pendingSize;
};

} // namespace QtWaylandClient

#endif // QWAYLANDLAYERSURFACE_P_H

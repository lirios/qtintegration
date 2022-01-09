// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef QWAYLANDLAYERSURFACE_P_H
#define QWAYLANDLAYERSURFACE_P_H

#include <QtWaylandClient/qtwaylandclientglobal.h>
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>

#include <LiriAuroraClient/WlrLayerSurfaceV1>

#include "qwaylandlayershell_p.h"

namespace QtWaylandClient {

class Q_WAYLAND_CLIENT_EXPORT QWaylandLayerSurface
        : public QWaylandShellSurface
        , public Aurora::Client::PrivateClient::zwlr_layer_surface_v1
{
    Q_OBJECT
public:
    QWaylandLayerSurface(QWaylandLayerShell *shell, QWaylandWindow *window);
    virtual ~QWaylandLayerSurface();

    bool isExposed() const override;

    void applyConfigure() override;
    void setWindowGeometry(const QRect &rect) override;

public Q_SLOTS:
    void setLayer(Aurora::Client::WlrLayerSurfaceV1::Layer layer);
    void setAnchors(Aurora::Client::WlrLayerSurfaceV1::Anchors anchors);
    void setSize(const QSize &surfaceSize);
    void setExclusiveZone(qint32 exclusiveZone);
    void setMargins(const QMargins &margins);
    void setKeyboardInteractivity(Aurora::Client::WlrLayerSurfaceV1::KeyboardInteractivity keyboardInteractivity);

protected:
    void zwlr_layer_surface_v1_configure(uint32_t serial, uint32_t width, uint32_t height) override;
    void zwlr_layer_surface_v1_closed() override;

private:
    QWaylandLayerShell *m_shell = nullptr;
    Aurora::Client::WlrLayerSurfaceV1::Anchors m_anchors;
    bool m_configured = false;
    QSize m_pendingSize;
};

} // namespace QtWaylandClient

#endif // QWAYLANDLAYERSURFACE_P_H

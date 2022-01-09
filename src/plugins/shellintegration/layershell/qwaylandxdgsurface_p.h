// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef QWAYLANDXDGSURFACE_H
#define QWAYLANDXDGSURFACE_H

#include <QtWaylandClient/qtwaylandclientglobal.h>
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>

#include "qwaylandxdgshell_p.h"

namespace QtWaylandClient {

class Q_WAYLAND_CLIENT_EXPORT QWaylandXdgSurface
        : public QWaylandShellSurface
        , public Aurora::Client::PrivateClient::xdg_surface
{
    Q_OBJECT
public:
    QWaylandXdgSurface(QWaylandXdgShell *xdgShell,
                       QWaylandWindow *window);
    virtual ~QWaylandXdgSurface();

    bool isExposed() const override;
    bool handleExpose(const QRegion &region) override;
    void applyConfigure() override;

protected:
    void xdg_surface_configure(uint32_t serial) override;

private:
    class Popup : public Aurora::Client::PrivateClient::xdg_popup
    {
    public:
        Popup(QWaylandXdgSurface *xdgSurface, struct ::xdg_popup *object);
        virtual ~Popup();

    protected:
        void xdg_popup_popup_done() override;

    private:
        QWaylandXdgSurface *m_xdgSurface = nullptr;
    };

    QWaylandXdgShell *m_xdgShell = nullptr;
    QWaylandWindow *m_window = nullptr;
    QWaylandXdgSurface::Popup *m_xdgPopup = nullptr;
    bool m_configured = false;
    uint32_t m_pendingConfigureSerial = 0;
    QRegion m_exposeRegion;
};

} // namespace QtWaylandClient

#endif // QWAYLANDXDGSURFACE_H

// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtWaylandClient/private/qwaylandinputdevice_p.h>
#include <QtWaylandClient/private/qwaylandscreen_p.h>
#include <QtWaylandClient/private/qwaylandsurface_p.h>

#include "qwaylandxdgsurface_p.h"
#include "qwaylandlayersurface_p.h"

namespace QtWaylandClient {

QWaylandXdgSurface::QWaylandXdgSurface(QWaylandXdgShell *xdgShell,
                                       QWaylandWindow *window)
    : QWaylandShellSurface(window)
    , Aurora::Client::PrivateClient::xdg_surface()
    , m_xdgShell(xdgShell)
    , m_window(window)
{
    // Initialize xdg surface
    auto *wlSurface = window->waylandSurface()->object();
    init(xdgShell->get_xdg_surface(wlSurface));

    // Create xdg popup positioner
    auto *xdgPositioner = new Aurora::Client::PrivateClient::xdg_positioner(xdgShell->create_positioner());
    auto pos = window->geometry().topLeft() - window->transientParent()->geometry().topLeft();
    xdgPositioner->set_anchor_rect(pos.x(), pos.y(), 1, 1);
    xdgPositioner->set_anchor(Aurora::Client::PrivateClient::xdg_positioner::anchor_top_left);
    xdgPositioner->set_gravity(Aurora::Client::PrivateClient::xdg_positioner::gravity_bottom_right);
    xdgPositioner->set_size(window->geometry().width(), window->geometry().height());

    // Create xdg popup
    m_xdgPopup = new QWaylandXdgSurface::Popup(this, get_popup(nullptr, xdgPositioner->object()));
    xdgPositioner->destroy();
    delete xdgPositioner;
}

QWaylandXdgSurface::~QWaylandXdgSurface()
{
    if (m_xdgPopup) {
        delete m_xdgPopup;
        m_xdgPopup = nullptr;
    }

    destroy();
}

bool QWaylandXdgSurface::isExposed() const
{
    return m_configured || m_pendingConfigureSerial;
}

bool QWaylandXdgSurface::handleExpose(const QRegion &region)
{
    if (!isExposed() && !region.isEmpty()) {
        m_exposeRegion = region;
        return true;
    }
    return false;
}

void QWaylandXdgSurface::applyConfigure()
{
    Q_ASSERT(m_pendingConfigureSerial != 0);

    m_configured = true;
    ack_configure(m_pendingConfigureSerial);

    m_pendingConfigureSerial = 0;
}

void QWaylandXdgSurface::xdg_surface_configure(uint32_t serial)
{
    m_pendingConfigureSerial = serial;

    if (m_configured) {
        // All configures after the initial one are for resizing the window,
        // let the QWaylandWindow machinery handle them when not painting
        window()->applyConfigureWhenPossible();
    } else {
        // Set the expose region to the window size
        applyConfigure();
        m_exposeRegion = QRegion(QRect(QPoint(), m_window->geometry().size()));
    }

    if (!m_exposeRegion.isEmpty()) {
        m_window->handleExpose(m_exposeRegion);
        m_exposeRegion = QRegion();
    }
}

QWaylandXdgSurface::Popup::Popup(QWaylandXdgSurface *xdgSurface, struct ::xdg_popup *object)
    : Aurora::Client::PrivateClient::xdg_popup(object)
    , m_xdgSurface(xdgSurface)
{
    auto *window = xdgSurface->m_window;

    // Set xdg popup as parent of the layer surface
    if (window->transientParent()) {
        auto *parent = static_cast<QWaylandLayerSurface *>(window->transientParent()->shellSurface());
        if (parent)
            parent->get_popup(object);
    }

    // Grab
    grab(window->display()->lastInputDevice()->wl_seat(),
         window->display()->lastInputSerial());
}

QWaylandXdgSurface::Popup::~Popup()
{
    destroy();
}

void QWaylandXdgSurface::Popup::xdg_popup_popup_done()
{
    m_xdgSurface->m_window->window()->close();
}

} // namespace QtWaylandClient

// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef QWAYLANDXDGSHELL_H
#define QWAYLANDXDGSHELL_H

#include <QtWaylandClient/qtwaylandclientglobal.h>

#include "qwayland-xdg-shell.h"

namespace QtWaylandClient {

class Q_WAYLAND_CLIENT_EXPORT QWaylandXdgShell : public QtWayland::xdg_wm_base
{
public:
    QWaylandXdgShell(struct ::wl_registry *registry, uint32_t id, uint32_t version);
    virtual ~QWaylandXdgShell();

protected:
    void xdg_wm_base_ping(uint32_t serial) override;
};

} // QtWaylandClient

#endif // QWAYLANDXDGSHELL_H

// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "qwaylandxdgshell_p.h"

namespace QtWaylandClient {

QWaylandXdgShell::QWaylandXdgShell(struct ::wl_registry *registry, uint32_t id, uint32_t version)
    : Aurora::Client::PrivateClient::xdg_wm_base(registry, id, version)
{
}

QWaylandXdgShell::~QWaylandXdgShell()
{
    xdg_wm_base_destroy(object());
}

void QWaylandXdgShell::xdg_wm_base_ping(uint32_t serial)
{
    pong(serial);
}

} // namespace QtWaylandClient {

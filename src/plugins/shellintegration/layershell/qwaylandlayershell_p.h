// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef QWAYLANDLAYERSHELL_P_H
#define QWAYLANDLAYERSHELL_P_H

#include <QtWaylandClient/qtwaylandclientglobal.h>

#include "aurora-client-wlr-layer-shell-unstable-v1.h"

namespace QtWaylandClient {

class Q_WAYLAND_CLIENT_EXPORT QWaylandLayerShell
        : public Aurora::Client::PrivateClient::zwlr_layer_shell_v1
{
public:
    QWaylandLayerShell(struct ::wl_registry *registry, uint32_t id, uint32_t version);
    virtual ~QWaylandLayerShell();
};

} // namespace QtWaylandClient

#endif //QWAYLANDLAYERSHELL_P_H

// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "qwaylandlayershell_p.h"

namespace QtWaylandClient {

QWaylandLayerShell::QWaylandLayerShell(struct ::wl_registry *registry, uint32_t id, uint32_t version)
    : QtWayland::zwlr_layer_shell_v1(registry, id, version)
{
}

QWaylandLayerShell::~QWaylandLayerShell()
{
    if (zwlr_layer_shell_v1_get_version(object()) >= ZWLR_LAYER_SHELL_V1_DESTROY_SINCE_VERSION)
        zwlr_layer_shell_v1_destroy(object());
}

} // namespace QtWaylandClient

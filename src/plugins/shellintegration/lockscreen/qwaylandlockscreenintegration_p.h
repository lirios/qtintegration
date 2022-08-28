// SPDX-FileCopyrightText: 2022 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef QWAYLANDLOCKSCREENINTEGRATION_P_H
#define QWAYLANDLOCKSCREENINTEGRATION_P_H

#include <QtWaylandClient/private/qwayland-wayland.h>
#include <QtWaylandClient/private/qwaylandshellintegration_p.h>

#include "qwaylandextsessionlock_p.h"

#include <wayland-client.h>

namespace QtWaylandClient {

class Q_WAYLAND_CLIENT_EXPORT QWaylandLockscreenIntegration
        : public QObject
        , public QWaylandShellIntegration
{
    Q_OBJECT
public:
    QWaylandLockscreenIntegration();

    bool initialize(QWaylandDisplay *display) override;
    QWaylandShellSurface *createShellSurface(QWaylandWindow *window) override;

private:
    QScopedPointer<QWaylandExtSessionLockManager> m_lockManager;
    QScopedPointer<QWaylandExtSessionLock> m_lock;

    void waitForLock();
};

} // namespace QtWaylandClient

#endif // QWAYLANDLOCKSCREENINTEGRATION_P_H

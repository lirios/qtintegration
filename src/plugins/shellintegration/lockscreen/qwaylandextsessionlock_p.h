// SPDX-FileCopyrightText: 2022 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef QWAYLANDEXTSESSIONLOCK_H
#define QWAYLANDEXTSESSIONLOCK_H

#include <QtWaylandClient/qtwaylandclientglobal.h>
#include <QtWaylandClient/private/qwayland-wayland.h>
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>

#include <LiriAuroraClient/ExtSessionLockSurfaceV1>

#include "aurora-client-ext-session-lock-v1.h"

namespace QtWaylandClient {

class QWaylandExtSessionLockManager;
class QWaylandExtSessionLock;

class Q_WAYLAND_CLIENT_EXPORT QWaylandExtSessionLockSurface
        : public QWaylandShellSurface
        , public Aurora::Client::PrivateClient::ext_session_lock_surface_v1
{
    Q_OBJECT
public:
    QWaylandExtSessionLockSurface(QWaylandExtSessionLock *lock, QWaylandWindow *window);
    virtual ~QWaylandExtSessionLockSurface();

    bool isExposed() const override;
    void applyConfigure() override;

protected:
    void ext_session_lock_surface_v1_configure(uint32_t serial, uint32_t width, uint32_t height) override;

private:
    bool m_configured = false;
    uint32_t m_pendingConfigureSerial = 0;
    QSize m_pendingSize;
    QWaylandExtSessionLock *m_lock = nullptr;

private Q_SLOTS:
    void handleUnlock();
};

class Q_WAYLAND_CLIENT_EXPORT QWaylandExtSessionLock
        : public Aurora::Client::PrivateClient::ext_session_lock_v1
{
public:
    QWaylandExtSessionLock(struct ::ext_session_lock_v1 *object, QWaylandExtSessionLockManager *manager);
    ~QWaylandExtSessionLock();

    QWaylandExtSessionLockSurface *createLockSurface(QWaylandWindow *window);

protected:
    void ext_session_lock_v1_locked() override;
    void ext_session_lock_v1_finished() override;

private:
    QWaylandExtSessionLockManager *m_manager;
};

class Q_WAYLAND_CLIENT_EXPORT QWaylandExtSessionLockManager
        : public QObject
        , public Aurora::Client::PrivateClient::ext_session_lock_manager_v1
{
    Q_OBJECT
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)
public:
    enum State {
        Initializing,
        Locking,
        Locked,
        Exiting
    };
    Q_ENUM(State)

    QWaylandExtSessionLockManager(struct ::wl_registry *registry, uint32_t id, uint32_t version);
    ~QWaylandExtSessionLockManager();

    State state() const;
    void setState(State state);

    QWaylandExtSessionLock *sendLock();

Q_SIGNALS:
    void stateChanged(QWaylandExtSessionLockManager::State state);
    void locked();
    void exitRequested();

private:
    State m_state = Initializing;
};

} // namespace QtWaylandClient

#endif // QWAYLANDEXTSESSIONLOCK_H

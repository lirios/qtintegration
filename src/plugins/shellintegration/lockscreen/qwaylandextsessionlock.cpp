// SPDX-FileCopyrightText: 2022 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtWaylandClient/private/qwaylandscreen_p.h>

#include "qwaylandextsessionlock_p.h"

namespace QtWaylandClient {

/*
 * QWaylandExtSessionLockSurface
 */

QWaylandExtSessionLockSurface::QWaylandExtSessionLockSurface(QWaylandExtSessionLock *lock, QWaylandWindow *window)
    : QWaylandShellSurface(window)
    , Aurora::Client::PrivateClient::ext_session_lock_surface_v1()
    , m_lock(lock)
{
    auto *interface = Aurora::Client::ExtSessionLockSurfaceV1::get(window->window());
    if (!interface) {
        qCWarning(lcQpaWayland) << "Cannot find LockSurface interface on window" << window->window();
        return;
    }

    init(lock->get_lock_surface(window->wlSurface(), window->waylandScreen()->output()));

    connect(interface, &Aurora::Client::ExtSessionLockSurfaceV1::unlockRequested,
            this, &QWaylandExtSessionLockSurface::handleUnlock);
}

QWaylandExtSessionLockSurface::~QWaylandExtSessionLockSurface()
{
    destroy();
}

bool QWaylandExtSessionLockSurface::isExposed() const
{
    return m_configured || m_pendingConfigureSerial;
}

void QWaylandExtSessionLockSurface::applyConfigure()
{
    Q_ASSERT(m_pendingConfigureSerial != 0);

    if (!m_pendingSize.isEmpty())
        window()->resizeFromApplyConfigure(m_pendingSize);

    m_configured = true;
    ack_configure(m_pendingConfigureSerial);

    m_pendingConfigureSerial = 0;
}

void QWaylandExtSessionLockSurface::ext_session_lock_surface_v1_configure(uint32_t serial, uint32_t width, uint32_t height)
{
    m_pendingConfigureSerial = serial;
    m_pendingSize = QSize(width, height);

    if (!m_configured) {
        // First time configuration
        applyConfigure();
    } else {
        // All configures after the initial one are for resizing the window,
        // let the QWaylandWindow machinery handle them when not painting
        window()->applyConfigureWhenPossible();
    }

    window()->handleExpose(QRect(QPoint(), m_pendingSize));
}

void QWaylandExtSessionLockSurface::handleUnlock()
{
    m_lock->unlock_and_destroy();
    window()->close();
}

/*
 * QWaylandExtSessionLock
 */

QWaylandExtSessionLock::QWaylandExtSessionLock(struct ::ext_session_lock_v1 *object, QWaylandExtSessionLockManager *manager)
    : Aurora::Client::PrivateClient::ext_session_lock_v1(object)
    , m_manager(manager)
{
}

QWaylandExtSessionLock::~QWaylandExtSessionLock()
{
    destroy();
}

QWaylandExtSessionLockSurface *QWaylandExtSessionLock::createLockSurface(QWaylandWindow *window)
{
    return new QWaylandExtSessionLockSurface(this, window);
}

void QWaylandExtSessionLock::ext_session_lock_v1_locked()
{
    m_manager->setState(QWaylandExtSessionLockManager::Locked);
}

void QWaylandExtSessionLock::ext_session_lock_v1_finished()
{
    switch (m_manager->state()) {
    case QWaylandExtSessionLockManager::Locking:
        qCWarning(lcQpaWayland, "The Wayland compositor has denied the attempt to lock the session: maybe another client is running, exiting...");
        m_manager->setState(QWaylandExtSessionLockManager::Exiting);
        break;
    case QWaylandExtSessionLockManager::Locked:
        qCWarning(lcQpaWayland, "The Wayland compositor has unlocked the session, exiting...");
        m_manager->setState(QWaylandExtSessionLockManager::Exiting);
        break;
    default:
        break;
    };
}

/*
 * QWaylandExtSessionLockManager
 */

QWaylandExtSessionLockManager::QWaylandExtSessionLockManager(struct ::wl_registry *registry, uint32_t id, uint32_t version)
    : QObject()
    , Aurora::Client::PrivateClient::ext_session_lock_manager_v1(registry, id, version)
{
}

QWaylandExtSessionLockManager::~QWaylandExtSessionLockManager()
{
    destroy();
}

QWaylandExtSessionLockManager::State QWaylandExtSessionLockManager::state() const
{
    return m_state;
}

void QWaylandExtSessionLockManager::setState(State state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit stateChanged(state);

    switch (state) {
    case Locked:
        emit locked();
        break;
    case Exiting:
        emit exitRequested();
        break;
    default:
        break;
    }
}

QWaylandExtSessionLock *QWaylandExtSessionLockManager::sendLock()
{
    m_state = Locking;
    return new QWaylandExtSessionLock(lock(), this);
}

} // namespace QtWaylandClient

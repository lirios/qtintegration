// SPDX-FileCopyrightText: 2022 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <QtGui/QGuiApplication>

#include "qwaylandlockscreenintegration_p.h"

namespace QtWaylandClient {

QWaylandLockscreenIntegration::QWaylandLockscreenIntegration()
    : QObject()
    , QWaylandShellIntegration()
{
}

bool QWaylandLockscreenIntegration::initialize(QWaylandDisplay *display)
{
    const auto lockManagerInterfaceName = QString::fromLatin1(QWaylandExtSessionLockManager::interface()->name);

    for (const QWaylandDisplay::RegistryGlobal &global : display->globals()) {
        if (global.interface == lockManagerInterfaceName && !m_lockManager) {
            m_lockManager.reset(new QWaylandExtSessionLockManager(display->wl_registry(), global.id, std::min(global.version, 1u)));
            break;
        }
    }

    if (!m_lockManager) {
        qCWarning(lcQpaWayland) << "Couldn't find global" << lockManagerInterfaceName << "for liri-lockscreen";
        return false;
    }

    // Quit when we fail to acquire the lock, see QWaylandExtSessionLock::ext_session_lock_v1_finished()
    connect(m_lockManager.data(), &QWaylandExtSessionLockManager::exitRequested,
            qGuiApp, &QGuiApplication::quit);

    // Ask the compositor to lock the session and wait for it, otherwise
    // createShellSurface() would fail since the lock is not yet acquired
    // when it's invoked
    m_lock.reset(m_lockManager->sendLock());
    waitForLock();

    return QWaylandShellIntegration::initialize(display);
}

QWaylandShellSurface *QWaylandLockscreenIntegration::createShellSurface(QWaylandWindow *window)
{
    if (window->window()->type() != Qt::Window) {
        qCWarning(lcQpaWayland) << "Only toplevel windows are supported by liri-lockscreen";
        return nullptr;
    }

    if (m_lockManager->state() != QWaylandExtSessionLockManager::Locked) {
        qCWarning(lcQpaWayland) << "A lock was not acquired: cannot create a lock surface";
        return nullptr;
    }

    return m_lock->createLockSurface(window);
}

void QWaylandLockscreenIntegration::waitForLock()
{
    QEventLoop loop;

    // We don't want to wait for ever: at some point we will give up
    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    connect(m_lockManager.data(), &QWaylandExtSessionLockManager::locked,
            &loop, &QEventLoop::quit);

    timer.start(5000);
    loop.exec();

    disconnect(m_lockManager.data(), nullptr, this, nullptr);
}

} // namespace QtWaylandClient

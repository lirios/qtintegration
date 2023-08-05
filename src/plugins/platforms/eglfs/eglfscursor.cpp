// SPDX-FileCopyrightText: 2023 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <LiriAuroraPlatform/Window>

#include "eglfscursor.h"
#include "eglfsdeviceintegration.h"
#include "eglfsscreen.h"

EglFSCursor::EglFSCursor(QPlatformScreen *screen)
    : QPlatformCursor()
    , m_screen(static_cast<EglFSScreen *>(screen))
{
}

EglFSCursor::~EglFSCursor()
{
}

#ifndef QT_NO_CURSOR
void EglFSCursor::changeCursor(QCursor *cursor, QWindow *qtWindow)
{
    auto *window = auroraDeviceIntegration()->getWindow(qtWindow);
    if (window)
        window->changeCursor(cursor);
}
#endif

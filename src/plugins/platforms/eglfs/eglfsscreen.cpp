// SPDX-FileCopyrightText: 2023 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <qpa/qwindowsysteminterface.h>

#include "eglfscursor.h"
#include "eglfsscreen.h"

EglFSScreen::EglFSScreen(Aurora::Platform::Output *output)
    : QObject()
    , QPlatformScreen()
    , m_output(output)
    , m_cursor(new EglFSCursor(this))
{
    connect(output, &Aurora::Platform::Output::modeChanged, this, [this]() {
        QWindowSystemInterface::handleScreenGeometryChange(screen(), m_output->geometry(),
                                                           m_output->geometry());
        QWindowSystemInterface::handleScreenRefreshRateChange(
                screen(), static_cast<qreal>(m_output->refreshRate()) / 1000.0);
    });
}

EglFSScreen::~EglFSScreen()
{
    if (m_cursor) {
        m_cursor->deleteLater();
        m_cursor = nullptr;
    }
}

Aurora::Platform::Output *EglFSScreen::auroraOutput() const
{
    return m_output.data();
}

QString EglFSScreen::name() const
{
    return m_output->name();
}

QString EglFSScreen::manufacturer() const
{
    return m_output->manufacturer();
}

QString EglFSScreen::model() const
{
    return m_output->model();
}

QString EglFSScreen::serialNumber() const
{
    return m_output->serialNumber();
}

QRect EglFSScreen::geometry() const
{
    return m_output->geometry();
}

qreal EglFSScreen::refreshRate() const
{
    return static_cast<qreal>(m_output->refreshRate()) / 1000.0;
}

int EglFSScreen::depth() const
{
    return m_output->depth();
}

QImage::Format EglFSScreen::format() const
{
    return m_output->format();
}

QSizeF EglFSScreen::physicalSize() const
{
    return m_output->physicalSize();
}

QDpi EglFSScreen::logicalDpi() const
{
    return QDpi();
}

qreal EglFSScreen::pixelDensity() const
{
    return qreal();
}

Qt::ScreenOrientation EglFSScreen::nativeOrientation() const
{
    return Qt::ScreenOrientation();
}

Qt::ScreenOrientation EglFSScreen::orientation() const
{
    return Qt::ScreenOrientation();
}

QPlatformCursor *EglFSScreen::cursor() const
{
    return m_cursor;
}

QPlatformScreen::SubpixelAntialiasingType EglFSScreen::subpixelAntialiasingTypeHint() const
{
    switch (m_output->subpixel()) {
    case Aurora::Platform::Output::Subpixel::HorizontalRGB:
        return Subpixel_RGB;
    case Aurora::Platform::Output::Subpixel::HorizontalBGR:
        return Subpixel_BGR;
    case Aurora::Platform::Output::Subpixel::VerticalRGB:
        return Subpixel_VRGB;
    case Aurora::Platform::Output::Subpixel::VerticalBGR:
        return Subpixel_VBGR;
    default:
        return Subpixel_None;
    }
}

QPixmap EglFSScreen::grabWindow(WId wid, int x, int y, int width, int height) const
{
    return QPixmap();
}

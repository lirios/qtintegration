// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// SPDX-FileCopyrightText: 2017 Michael Spencer <sonrisesoftware@gmail.com>
// SPDX-FileCopyrightText: 2016 Robin Burchell <robin.burchell@viroteck.net>
// SPDX-FileCopyrightText: 2016 The Qt Company Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-only OR (GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL)

#include <QtMath>
#include <QPainterPath>

#include "materialdecoration.h"

using namespace Qt::StringLiterals;

QT_BEGIN_NAMESPACE

namespace QtWaylandClient {

static constexpr int ceButtonWidth = 24;
static constexpr int ceButtonSpacing = 12;
static constexpr int ceTitlebarHeight = 32;
static constexpr int ceTitleFontSize = 16;
static constexpr int ceWindowBorderWidth = 4;

QWaylandMaterialDecoration::QWaylandMaterialDecoration()
    : QWaylandAbstractDecoration()
    , m_clicking(None)
    , m_backgroundColor("#455a64")
    , m_iconColor("#b4ffffff")
    , m_textColor("#ffffff")
{
    QTextOption option(Qt::AlignHCenter | Qt::AlignVCenter);
    option.setWrapMode(QTextOption::NoWrap);
    m_windowTitle.setTextOption(option);
}

QColor QWaylandMaterialDecoration::backgroundColor() const
{
    return m_backgroundColor;
}

void QWaylandMaterialDecoration::setBackgroundColor(const QColor &color)
{
    if (color == m_backgroundColor)
        return;

    m_backgroundColor = color;
    update();
    Q_EMIT backgroundColorChanged();
}

QColor QWaylandMaterialDecoration::textColor() const
{
    return m_textColor;
}

void QWaylandMaterialDecoration::setTextColor(const QColor &color)
{
    if (color == m_textColor)
        return;

    m_textColor = color;
    update();
    Q_EMIT textColorChanged();
}

QColor QWaylandMaterialDecoration::iconColor() const
{
    return m_iconColor;
}

void QWaylandMaterialDecoration::setIconColor(const QColor &color)
{
    if (color == m_iconColor)
        return;

    m_iconColor = color;
    update();
    Q_EMIT iconColorChanged();
}

QRectF QWaylandMaterialDecoration::closeButtonRect() const
{
    return QRectF(window()->frameGeometry().width() - ceButtonWidth - ceButtonSpacing,
                  (margins().top() - ceButtonWidth) / 2, ceButtonWidth, ceButtonWidth);
}

QRectF QWaylandMaterialDecoration::maximizeButtonRect() const
{
    return QRectF(window()->frameGeometry().width() - ceButtonWidth * 2 - ceButtonSpacing * 2,
                  (margins().top() - ceButtonWidth) / 2, ceButtonWidth, ceButtonWidth);
}

QRectF QWaylandMaterialDecoration::minimizeButtonRect() const
{
    return QRectF(window()->frameGeometry().width() - ceButtonWidth * 3 - ceButtonSpacing * 3,
                  (margins().top() - ceButtonWidth) / 2, ceButtonWidth, ceButtonWidth);
}

QMargins QWaylandMaterialDecoration::margins(MarginsType marginsType) const
{
    if (marginsType == ShadowsOnly)
        return QMargins();

    // Title bar is 32dp plus borders
    if (window() && window()->type() == Qt::Popup)
        return QMargins(0, 0, 0, 0);
    if (window()
        && ((window()->windowStates() & Qt::WindowMaximized)
            || (window()->windowStates() & Qt::WindowFullScreen)))
        return QMargins(0, ceTitlebarHeight, 0, 0);
    return QMargins(ceWindowBorderWidth, ceTitlebarHeight, ceWindowBorderWidth,
                    ceWindowBorderWidth);
}

void QWaylandMaterialDecoration::paint(QPaintDevice *device)
{
    // Set decoration colors of Fluid applications
    if (window()) {
        QVariant bgColor = window()->property("__material_decoration_backgroundColor");
        if (bgColor.isValid()) {
            QColor color = bgColor.value<QColor>();
            if (color != m_backgroundColor) {
                m_backgroundColor = color;
                Q_EMIT backgroundColorChanged();
            }
        }

        QVariant fgColor = window()->property("__material_decoration_foregroundColor");
        if (fgColor.isValid()) {
            QColor color = fgColor.value<QColor>();
            if (color != m_textColor) {
                m_textColor = color;
                Q_EMIT textColorChanged();
                m_iconColor = color;
                Q_EMIT iconColorChanged();
            }
        }
    }

    const QRect frameGeometry = window()->frameGeometry();
    QRect top(QPoint(ceWindowBorderWidth, ceWindowBorderWidth),
              QSize(frameGeometry.width(), margins().top()));

    QPainter p(device);
    p.setRenderHint(QPainter::Antialiasing);

    // Title bar
    int radius = window()->windowStates() & Qt::WindowMaximized ? 0 : dp(3);
    QPainterPath roundedRect;
    roundedRect.addRoundedRect(margins().left(), margins().top() - ceTitlebarHeight,
                               frameGeometry.width() - margins().left() - margins().right(),
                               ceTitlebarHeight + radius * 2, radius, radius);
    p.fillPath(roundedRect.simplified(), m_backgroundColor);

    // Borders (transparent so the border is not noticeable)
    QPainterPath borderPath;
    borderPath.addRect(0, margins().top(), margins().left(),
                       frameGeometry.height() - margins().top());
    borderPath.addRect(0, frameGeometry.height() - margins().bottom(), frameGeometry.width(),
                       margins().bottom());
    borderPath.addRect(frameGeometry.width() - margins().right(), margins().top(),
                       margins().right(), frameGeometry.height() - margins().bottom());
    p.fillPath(borderPath, Qt::transparent);

    // Window title
    QString windowTitleText = window()->title();
    if (!windowTitleText.isEmpty()) {
        if (m_windowTitle.text() != windowTitleText) {
            m_windowTitle.setText(windowTitleText);
            m_windowTitle.prepare();
        }

        QRect titleBar = top;
        titleBar.setLeft(margins().left() + ceButtonSpacing);
        titleBar.setRight(qFloor(minimizeButtonRect().left()) - ceButtonSpacing);

        p.save();
        p.setClipRect(titleBar);
        p.setPen(m_textColor);
        QSizeF size = m_windowTitle.size();
        int dx = qFloor((top.width() - size.width()) / 2);
        int dy = qFloor((top.height() - ceTitleFontSize) / 2);
        QFont font = p.font();
        font.setBold(true);
        font.setFamily("Roboto"_L1);
        font.setPixelSize(ceTitleFontSize);
        p.setFont(font);
        QPoint windowTitlePoint(dx, dy - ceWindowBorderWidth / 2);
        p.drawStaticText(windowTitlePoint, m_windowTitle);
        p.restore();
    }

    // Buttons
    {
        QRectF rect;
        QPen pen(m_iconColor);
        p.setPen(pen);

        // Close button
        p.save();
        rect = closeButtonRect();
        qreal crossSize = rect.height() / 2.3;
        QPointF crossCenter(rect.center());
        QRectF crossRect(crossCenter.x() - crossSize / 2, crossCenter.y() - crossSize / 2,
                         crossSize, crossSize);
        pen.setWidth(2);
        p.setPen(pen);
        p.drawLine(crossRect.topLeft(), crossRect.bottomRight());
        p.drawLine(crossRect.bottomLeft(), crossRect.topRight());
        p.restore();

        // Maximize button
        if (isMaximizeable()) {
            p.save();
            pen.setWidth(2);
            p.setPen(pen);
            if (window()->windowStates() & Qt::WindowMaximized) {
                p.setRenderHint(QPainter::Antialiasing, true);

                rect = maximizeButtonRect().adjusted(4, 5, -4, -5);

                qreal inset = 4;
                QRectF rect1 = rect.adjusted(inset, 0, 0, -inset);
                QRectF rect2 = rect.adjusted(0, inset, -inset, 0);
                p.drawRect(rect1);
                p.setBrush(m_backgroundColor); // need to cover up some lines from the other rect
                p.drawRect(rect2);
                p.drawLine(rect2.topLeft() + QPointF(2, 2), rect2.topRight() + QPointF(-2, 2));
            } else {
                p.setRenderHint(QPainter::Antialiasing, false);

                rect = maximizeButtonRect().adjusted(5, 5, -5, -5);

                QVector<QLineF> lines;
                lines.append(QLineF(rect.topLeft(), rect.topRight()));
                lines.append(QLineF(rect.left(), rect.top() + 2, rect.left(), rect.bottom() - 2));
                lines.append(QLineF(rect.right(), rect.top() + 2, rect.right(), rect.bottom() - 2));
                lines.append(QLineF(rect.bottomLeft(), rect.bottomRight()));
                lines.append(
                        QLineF(rect.left() + 2, rect.top() + 2, rect.right() - 2, rect.top() + 2));
                p.drawLines(lines);
            }
            p.restore();
        }

        // Minimize button
        if (isMinimizeable()) {
            p.save();
            p.setRenderHint(QPainter::Antialiasing, false);
            rect = minimizeButtonRect().adjusted(5, 5, -5, -5);
            pen.setWidth(2);
            p.setPen(pen);
            p.drawLine(rect.bottomLeft(), rect.bottomRight());
            p.restore();
        }
    }
}

bool QWaylandMaterialDecoration::clickButton(Qt::MouseButtons b, Button btn)
{
    if (isLeftClicked(b)) {
        m_clicking = btn;
        return false;
    } else if (isLeftReleased(b)) {
        if (m_clicking == btn) {
            m_clicking = None;
            return true;
        } else {
            m_clicking = None;
        }
    }
    return false;
}

bool QWaylandMaterialDecoration::handleMouse(QWaylandInputDevice *inputDevice, const QPointF &local,
                                             const QPointF &global, Qt::MouseButtons b,
                                             Qt::KeyboardModifiers mods)

{
    Q_UNUSED(global);

    // Figure out what area mouse is in
    if (closeButtonRect().contains(local)) {
        if (clickButton(b, Close))
            QWindowSystemInterface::handleCloseEvent(window());
    } else if (isMaximizeable() && maximizeButtonRect().contains(local)) {
        if (clickButton(b, Maximize))
            window()->setWindowState(window()->windowStates() & Qt::WindowMaximized
                                             ? Qt::WindowNoState
                                             : Qt::WindowMaximized);
    } else if (minimizeButtonRect().contains(local)) {
        if (clickButton(b, Minimize))
            window()->setWindowState(Qt::WindowMinimized);
    } else if (local.y() <= margins().top()) {
        processMouseTop(inputDevice, local, b, mods);
    } else if (local.y() > window()->height() - margins().bottom() + margins().top()) {
        processMouseBottom(inputDevice, local, b, mods);
    } else if (local.x() <= margins().left()) {
        processMouseLeft(inputDevice, local, b, mods);
    } else if (local.x() > window()->width() - margins().right() + margins().left()) {
        processMouseRight(inputDevice, local, b, mods);
    } else {
        waylandWindow()->restoreMouseCursor(inputDevice);
        setMouseButtons(b);
        return false;
    }

    setMouseButtons(b);
    return true;
}

bool QWaylandMaterialDecoration::handleTouch(QWaylandInputDevice *inputDevice, const QPointF &local,
                                             const QPointF &global, QEventPoint::State state,
                                             Qt::KeyboardModifiers mods)
{
    Q_UNUSED(inputDevice);
    Q_UNUSED(global);
    Q_UNUSED(mods);

    bool handled = state == QEventPoint::Pressed;
    if (handled) {
        if (closeButtonRect().contains(local))
            QWindowSystemInterface::handleCloseEvent(window());
        else if (isMaximizeable() && maximizeButtonRect().contains(local))
            window()->setWindowState(window()->windowStates() & Qt::WindowMaximized
                                             ? Qt::WindowNoState
                                             : Qt::WindowMaximized);
        else if (minimizeButtonRect().contains(local))
            window()->setWindowState(Qt::WindowMinimized);
        else if (local.y() <= margins().top())
            waylandWindow()->shellSurface()->move(inputDevice);
        else
            handled = false;
    }

    return handled;
}

void QWaylandMaterialDecoration::processMouseTop(QWaylandInputDevice *inputDevice,
                                                 const QPointF &local, Qt::MouseButtons b,
                                                 Qt::KeyboardModifiers mods)
{
    Q_UNUSED(mods);
    if (local.y() <= margins().bottom()) {
        if (local.x() <= margins().left()) {
            // top left bit
            waylandWindow()->setMouseCursor(inputDevice, Qt::SizeFDiagCursor);
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
            startResize(inputDevice, Qt::TopEdge | Qt::LeftEdge, b);
#else
            startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_TOP_LEFT, b);
#endif
        } else if (local.x() > window()->width() - margins().right()) {
            // top right bit
            waylandWindow()->setMouseCursor(inputDevice, Qt::SizeBDiagCursor);
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
            startResize(inputDevice, Qt::TopEdge | Qt::RightEdge, b);
#else
            startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_TOP_RIGHT, b);
#endif
        } else {
            // top reszie bit
            waylandWindow()->setMouseCursor(inputDevice, Qt::SplitVCursor);
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
            startResize(inputDevice, Qt::TopEdge, b);
#else
            startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_TOP, b);
#endif
        }
    } else {
        waylandWindow()->restoreMouseCursor(inputDevice);
        startMove(inputDevice, b);
    }
}

void QWaylandMaterialDecoration::processMouseBottom(QWaylandInputDevice *inputDevice,
                                                    const QPointF &local, Qt::MouseButtons b,
                                                    Qt::KeyboardModifiers mods)
{
    Q_UNUSED(mods);

    if (local.x() <= margins().left()) {
        // bottom left bit
        waylandWindow()->setMouseCursor(inputDevice, Qt::SizeBDiagCursor);
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
        startResize(inputDevice, Qt::BottomEdge | Qt::LeftEdge, b);
#else
        startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_BOTTOM_LEFT, b);
#endif
    } else if (local.x() > window()->width() - margins().right()) {
        // bottom right bit
        waylandWindow()->setMouseCursor(inputDevice, Qt::SizeFDiagCursor);
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
        startResize(inputDevice, Qt::BottomEdge | Qt::RightEdge, b);
#else
        startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_BOTTOM_RIGHT, b);
#endif
    } else {
        // bottom bit
        waylandWindow()->setMouseCursor(inputDevice, Qt::SplitVCursor);
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
        startResize(inputDevice, Qt::BottomEdge, b);
#else
        startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_BOTTOM, b);
#endif
    }
}

void QWaylandMaterialDecoration::processMouseLeft(QWaylandInputDevice *inputDevice,
                                                  const QPointF &local, Qt::MouseButtons b,
                                                  Qt::KeyboardModifiers mods)
{
    Q_UNUSED(local);
    Q_UNUSED(mods);

    waylandWindow()->setMouseCursor(inputDevice, Qt::SplitHCursor);
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
    startResize(inputDevice, Qt::LeftEdge, b);
#else
    startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_LEFT, b);
#endif
}

void QWaylandMaterialDecoration::processMouseRight(QWaylandInputDevice *inputDevice,
                                                   const QPointF &local, Qt::MouseButtons b,
                                                   Qt::KeyboardModifiers mods)
{
    Q_UNUSED(local);
    Q_UNUSED(mods);
    waylandWindow()->setMouseCursor(inputDevice, Qt::SplitHCursor);
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
    startResize(inputDevice, Qt::RightEdge, b);
#else
    startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_RIGHT, b);
#endif
}

int QWaylandMaterialDecoration::dp(int dp) const
{
    return dp;
}

bool QWaylandMaterialDecoration::isMinimizeable() const
{
    return window()->flags() & Qt::WindowMinimizeButtonHint || window()->isTopLevel();
}

bool QWaylandMaterialDecoration::isMaximizeable() const
{
    return window()->flags() & Qt::WindowMaximizeButtonHint
            || ((window()->maximumSize().width() > window()->minimumSize().width())
                && (window()->maximumSize().height() > window()->minimumSize().height()));
}

} // namespace QtWaylandClient

QT_END_NAMESPACE

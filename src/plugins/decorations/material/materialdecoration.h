// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// SPDX-FileCopyrightText: 2017 Michael Spencer <sonrisesoftware@gmail.com>
// SPDX-FileCopyrightText: 2016 Robin Burchell <robin.burchell@viroteck.net>
// SPDX-FileCopyrightText: 2016 The Qt Company Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-only OR (GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL)

#include <QBitmap>
#include <QtGui/QCursor>
#include <QtGui/QLinearGradient>
#include <QtGui/QPainter>
#include <QtGui/QPalette>

#include <QtGui/qpa/qwindowsysteminterface.h>

#include <QtWaylandClient/private/qwaylandabstractdecoration_p.h>
#include <QtWaylandClient/private/qwaylanddecorationplugin_p.h>
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>

QT_BEGIN_NAMESPACE

namespace QtWaylandClient {

enum Button {
    None,
    Close,
    Maximize,
    Minimize
};

class Q_WAYLANDCLIENT_EXPORT QWaylandMaterialDecoration : public QWaylandAbstractDecoration
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY
                       backgroundColorChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QColor iconColor READ iconColor WRITE setIconColor NOTIFY iconColorChanged)
public:
    QWaylandMaterialDecoration();

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    QColor textColor() const;
    void setTextColor(const QColor &color);

    QColor iconColor() const;
    void setIconColor(const QColor &color);

Q_SIGNALS:
    void backgroundColorChanged();
    void textColorChanged();
    void iconColorChanged();

protected:
    QMargins margins(MarginsType marginsType = Full) const override;

    void paint(QPaintDevice *device) override;

    bool handleMouse(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global,
                     Qt::MouseButtons b, Qt::KeyboardModifiers mods) override;
    bool handleTouch(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global,
                     QEventPoint::State state, Qt::KeyboardModifiers mods) override;

private:
    void processMouseTop(QWaylandInputDevice *inputDevice, const QPointF &local, Qt::MouseButtons b,
                         Qt::KeyboardModifiers mods);
    void processMouseBottom(QWaylandInputDevice *inputDevice, const QPointF &local,
                            Qt::MouseButtons b, Qt::KeyboardModifiers mods);
    void processMouseLeft(QWaylandInputDevice *inputDevice, const QPointF &local,
                          Qt::MouseButtons b, Qt::KeyboardModifiers mods);
    void processMouseRight(QWaylandInputDevice *inputDevice, const QPointF &local,
                           Qt::MouseButtons b, Qt::KeyboardModifiers mods);
    bool clickButton(Qt::MouseButtons b, Button btn);

    QRectF closeButtonRect() const;
    QRectF maximizeButtonRect() const;
    QRectF minimizeButtonRect() const;

    int dp(int dp) const;

    bool isMinimizeable() const;
    bool isMaximizeable() const;

    Button m_clicking;
    QColor m_backgroundColor;
    QColor m_iconColor;
    QColor m_textColor;
    QStaticText m_windowTitle;
};
} // namespace QtWaylandClient

// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "materialdecoration.h"

using namespace QtWaylandClient;

class QWaylandMaterialDecorationPlugin : public QWaylandDecorationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QWaylandDecorationFactoryInterface_iid FILE "material.json")
public:
    QWaylandAbstractDecoration *create(const QString &key, const QStringList &paramList) override
    {
        Q_UNUSED(key);
        Q_UNUSED(paramList);
        return new QWaylandMaterialDecoration();
    }
};

#include "plugin.moc"

// SPDX-FileCopyrightText: 2023 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QtEglSupport/private/qeglconvenience_p.h>

class EglFSConfigChooser : public QEglConfigChooser
{
public:
    EglFSConfigChooser(EGLDisplay display);
};

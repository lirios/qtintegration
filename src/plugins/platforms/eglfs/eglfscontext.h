// SPDX-FileCopyrightText: 2023 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QVariant>

#include <QtEglSupport/private/qeglplatformcontext_p.h>

class EglFSContext : public QEGLPlatformContext
{
public:
    EglFSContext(const QSurfaceFormat &format, QPlatformOpenGLContext *share, EGLDisplay display,
                 EGLConfig *config, const QVariant &nativeHandle = QVariant(), Flags flags = {});

    void swapBuffers(QPlatformSurface *surface) override;

protected:
    EGLSurface eglSurfaceForPlatformSurface(QPlatformSurface *surface) override;
    void runGLChecks() override;
};

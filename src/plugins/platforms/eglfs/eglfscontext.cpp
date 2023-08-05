// SPDX-FileCopyrightText: 2023 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QSurface>

#include <QtEglSupport/private/qeglconvenience_p.h>
#include <QtEglSupport/private/qeglpbuffer_p.h>

#include "eglfscategories.h"
#include "eglfscontext.h"
#include "eglfsdeviceintegration.h"
#include "eglfswindow.h"

EglFSContext::EglFSContext(const QSurfaceFormat &format, QPlatformOpenGLContext *share,
                           EGLDisplay display, EGLConfig *config, const QVariant &nativeHandle,
                           Flags flags)
    : QEGLPlatformContext(format, share, display, config, nativeHandle,
                          auroraDeviceIntegration()->supportsSurfacelessContexts()
                                  ? Flags()
                                  : QEGLPlatformContext::NoSurfaceless)
{
}

void EglFSContext::swapBuffers(QPlatformSurface *surface)
{
    auto *platformWindow = static_cast<EglFSWindow *>(surface);
    if (platformWindow) {
        auroraDeviceIntegration()->waitForVSync(platformWindow->auroraWindow());
        QEGLPlatformContext::swapBuffers(surface);
        auroraDeviceIntegration()->presentBuffer(platformWindow->auroraWindow());
    }
}

EGLSurface EglFSContext::eglSurfaceForPlatformSurface(QPlatformSurface *surface)
{
    if (surface->surface()->surfaceClass() == QSurface::Window)
        return static_cast<EglFSWindow *>(surface)->surface();
    else
        return static_cast<QEGLPbuffer *>(surface)->pbuffer();
}

void EglFSContext::runGLChecks()
{
    // Beware that QOpenGLContext and QOpenGLFunctions are not yet usable at this stage

    const char *renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
    if (renderer) {
        // Warn about unsupported or limited hardware
        if (strstr(renderer, "llvmpipe"))
            qCWarning(gLcEglFS,
                      "Running on a software rasterizer (LLVMpipe): performance will be limited");
    }
}

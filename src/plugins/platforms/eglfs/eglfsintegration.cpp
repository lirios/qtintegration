// SPDX-FileCopyrightText: 2023 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QEventLoop>
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QScreen>
#ifndef QT_NO_OPENGL
#  include <QOpenGLContext>
#  include <QOffscreenSurface>
#endif

#include <QtGui/private/qguiapplication_p.h>

#include <qpa/qplatformwindow.h>
#include <qpa/qwindowsysteminterface.h>
#include <qpa/qplatforminputcontextfactory_p.h>

#ifndef QT_NO_OPENGL
#  include <QtEglSupport/private/qeglpbuffer_p.h>
#endif
#include <QtFontDatabaseSupport/private/qgenericunixfontdatabase_p.h>
#include <QtServiceSupport/private/qgenericunixservices_p.h>
#include <QtThemeSupport/private/qgenericunixthemes_p.h>
#include <QtEventDispatcherSupport/private/qgenericunixeventdispatcher_p.h>

#include <QtPlatformHeaders/QEGLNativeContext>

#include "eglfscategories.h"
#include "eglfsdeviceintegration.h"
#include "eglfsinfo.h"
#include "eglfsinputmanager.h"
#include "eglfsintegration.h"
#include "eglfsscreen.h"
#include "eglfswindow.h"

EglFSIntegration::EglFSIntegration(const EglFSIntegrationOptions &options)
    : QPlatformIntegration()
    , QPlatformNativeInterface()
    , m_options(options)
    , m_loop(new QEventLoop(this))
    , m_fontDb(new QGenericUnixFontDatabase())
    , m_services(new QGenericUnixServices())
{
    auto *integration = auroraDeviceIntegration();
    connect(integration, &Aurora::Platform::DeviceIntegration::statusChanged, this,
            &EglFSIntegration::handleIntegrationStatusChanged, Qt::QueuedConnection);
    connect(integration, &Aurora::Platform::DeviceIntegration::outputAdded, this,
            &EglFSIntegration::handleOutputAdded);
    connect(integration, &Aurora::Platform::DeviceIntegration::outputRemoved, this,
            &EglFSIntegration::handleOutputRemoved);
}

EglFSIntegration::~EglFSIntegration()
{
}

void EglFSIntegration::initialize()
{
    auroraDeviceIntegration()->initialize();

    // Wait until the integration is ready
    m_loop->exec();
    if (!m_ready) {
        destroy();
        qFatal("Aborting...");
    }

    // Create EGL display
    auto nativeDisplay = auroraDeviceIntegration()->platformDisplay();
    m_display = auroraDeviceIntegration()->eglDisplay();
    if (Q_UNLIKELY(m_display == EGL_NO_DISPLAY))
        qFatal("Failed to open EGL display, cannot continue");

    // Initialize EGL
    EGLint major, minor;
    if (Q_UNLIKELY(!eglInitialize(m_display, &major, &minor)))
        qFatal("Could not initialize EGL display, cannot continue");

    m_inputContext = QPlatformInputContextFactory::create();

    if (!m_options.disableInputHandlers)
        m_inputManager.reset(new EglFSInputManager());
}

void EglFSIntegration::destroy()
{
    qCDebug(gLcEglFS) << "Device integration is about to be destroyed...";

    auroraDeviceIntegration()->destroy();

    m_display = EGL_NO_DISPLAY;
}

EGLDisplay EglFSIntegration::display() const
{
    return m_display;
}

QAbstractEventDispatcher *EglFSIntegration::createEventDispatcher() const
{
    return createUnixEventDispatcher();
}

QPlatformFontDatabase *EglFSIntegration::fontDatabase() const
{
    return m_fontDb.data();
}

QPlatformServices *EglFSIntegration::services() const
{
    return m_services.data();
}

QPlatformInputContext *EglFSIntegration::inputContext() const
{
    return m_inputContext;
}

QPlatformTheme *EglFSIntegration::createPlatformTheme(const QString &name) const
{
    return QGenericUnixTheme::createUnixTheme(name);
}

QPlatformWindow *EglFSIntegration::createPlatformWindow(QWindow *window) const
{
    QWindowSystemInterface::flushWindowSystemEvents(QEventLoop::ExcludeUserInputEvents);

    // Only toplevel windows with OpenGL surfaces are allowed since this platform plugin is
    // tailored for Aurora Wayland compositors that all use QtQuick
    if (window->transientParent()) {
        qCWarning(gLcEglFS) << "Window" << window
                            << "has a transient parent: only toplevel windows are allowed";
        return nullptr;
    }
    if (window->surfaceType() == QWindow::SurfaceType::RasterSurface
        || window->surfaceType() == QWindow::SurfaceType::RasterGLSurface) {
        qCWarning(gLcEglFS) << "Window" << window << "has a raster surface which is not allowed";
        return nullptr;
    }

    // Only a window for screen is allowed
    const auto topLevelWindows = QGuiApplication::topLevelWindows();
    for (auto *topLevelWindow : topLevelWindows) {
        if (topLevelWindow != window && topLevelWindow->screen() == window->screen()) {
            qCWarning(gLcEglFS) << "Window" << window
                                << "cannot be created: there is already a window on the same screen"
                                << window->screen()->name();
            return nullptr;
        }
    }

    // Create the platform window
    auto *platformWindow = new EglFSWindow(const_cast<EglFSIntegration *>(this), window);
    if (!platformWindow->create()) {
        delete platformWindow;
        return nullptr;
    }

    // Show without activating
    const auto showWithoutActivating = window->property("_q_showWithoutActivating");
    if (showWithoutActivating.isValid() && showWithoutActivating.toBool())
        return platformWindow;

    // Activate only the window for the primary screen to make input work
    if (window->type() != Qt::ToolTip && window->screen() == QGuiApplication::primaryScreen())
        platformWindow->requestActivateWindow();

    return platformWindow;
}

QPlatformBackingStore *EglFSIntegration::createPlatformBackingStore(QWindow *window) const
{
    return nullptr;
}

#ifndef QT_NO_OPENGL
QPlatformOpenGLContext *EglFSIntegration::createPlatformOpenGLContext(QOpenGLContext *context) const
{
    QPlatformOpenGLContext *share = context->shareHandle();
    QVariant nativeHandle = context->nativeHandle();

    EglFSContext *ctx = nullptr;
    EGLConfig config = nullptr;
    QSurfaceFormat adjustedFormat = auroraDeviceIntegration()->surfaceFormatFor(context->format());

    if (nativeHandle.isNull()) {
        config = auroraDeviceIntegration()->chooseConfig(m_display, adjustedFormat);
        ctx = new EglFSContext(adjustedFormat, share, m_display, &config, QVariant());
    } else {
        ctx = new EglFSContext(adjustedFormat, share, m_display, nullptr, nativeHandle);
    }

    nativeHandle =
            QVariant::fromValue<QEGLNativeContext>(QEGLNativeContext(ctx->eglContext(), m_display));

    context->setNativeHandle(nativeHandle);

    // Print some information
    eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, ctx->eglContext());
    logGLInfo();
    logEGLInfo(m_display);
    if (config)
        logEGLConfigInfo(m_display, config);
    eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    return ctx;
}

QPlatformOffscreenSurface *
EglFSIntegration::createPlatformOffscreenSurface(QOffscreenSurface *surface) const
{
    return nullptr;
}
#endif

#if QT_CONFIG(vulkan)
QPlatformVulkanInstance *
EglFSIntegration::createPlatformVulkanInstance(QVulkanInstance *instance) const
{
    return nullptr;
}
#endif

bool EglFSIntegration::hasCapability(QPlatformIntegration::Capability cap) const
{
    switch (cap) {
    case ThreadedPixmaps:
        return true;
#ifndef QT_NO_OPENGL
    case OpenGL:
        return true;
    case ThreadedOpenGL:
        return true;
    case RasterGLSurface:
        return false;
#else
    case OpenGL:
        return false;
    case ThreadedOpenGL:
        return false;
    case RasterGLSurface:
        return false;
#endif
    case WindowMasks:
        return false;
    case MultipleWindows:
        return true;
    case ForeignWindows:
        return false;
    case WindowManagement:
        return false;
    case WindowActivation:
        return true;
    case MaximizeUsingFullscreenGeometry:
        return true;
    case NonFullScreenWindows:
        return false;
    case OpenGLOnRasterSurface:
        return false;
    case ApplicationState:
        return false;
    default:
        return QPlatformIntegration::hasCapability(cap);
    }
}

QPlatformNativeInterface *EglFSIntegration::nativeInterface() const
{
    return const_cast<EglFSIntegration *>(this);
}

void *EglFSIntegration::nativeResourceForIntegration(const QByteArray &resource)
{
    return nullptr;
}

void *EglFSIntegration::nativeResourceForScreen(const QByteArray &resource, QScreen *screen)
{
    return nullptr;
}

void *EglFSIntegration::nativeResourceForWindow(const QByteArray &resource, QWindow *window)
{
    return nullptr;
}

#ifndef QT_NO_OPENGL
void *EglFSIntegration::nativeResourceForContext(const QByteArray &resource,
                                                 QOpenGLContext *context)
{
    return nullptr;
}
#endif

QPlatformNativeInterface::NativeResourceForContextFunction
EglFSIntegration::nativeResourceFunctionForContext(const QByteArray &resource)
{
    return NativeResourceForContextFunction();
}

QFunctionPointer EglFSIntegration::platformFunction(const QByteArray &function) const
{
    return QFunctionPointer();
}

void EglFSIntegration::handleIntegrationStatusChanged(
        Aurora::Platform::DeviceIntegration::Status status)
{
    switch (status) {
    case Aurora::Platform::DeviceIntegration::Status::Ready:
        qCInfo(gLcEglFS) << "Device integration initialized successfully";
        m_ready = true;
        m_loop->quit();
        break;
    case Aurora::Platform::DeviceIntegration::Status::Failed:
        qCWarning(gLcEglFS) << "Device integration initialization failed";
        m_ready = false;
        m_loop->quit();
        break;
    default:
        break;
    }
}

void EglFSIntegration::handleOutputAdded(Aurora::Platform::Output *output)
{
    const auto isPrimary = QGuiApplication::screens().length() == 0;
    auto *platformScreen = new EglFSScreen(output);
    QWindowSystemInterface::handleScreenAdded(platformScreen, isPrimary);
    qCDebug(gLcEglFS) << "Creating screen" << platformScreen << "(" << platformScreen->name() << ")"
                      << "with geometry" << platformScreen->geometry() << "and isPrimary"
                      << isPrimary;
}

void EglFSIntegration::handleOutputRemoved(Aurora::Platform::Output *output)
{
    const auto screens = QGuiApplication::screens();
    for (auto *screen : qAsConst(screens)) {
        auto *platformScreen = static_cast<EglFSScreen *>(screen->handle());
        if (platformScreen && platformScreen->auroraOutput() == output) {
            QWindowSystemInterface::handleScreenRemoved(platformScreen);
            break;
        }
    }
}

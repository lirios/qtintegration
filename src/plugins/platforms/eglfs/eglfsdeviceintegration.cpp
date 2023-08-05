// SPDX-FileCopyrightText: 2023 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <LiriAuroraPlatform/DeviceIntegrationFactory>

#include "eglfscategories.h"
#include "eglfsdeviceintegration.h"

class EglFSDeviceIntegration
{
public:
    EglFSDeviceIntegration();
    ~EglFSDeviceIntegration();

    Aurora::Platform::DeviceIntegration *deviceIntegration() const;

private:
    Aurora::Platform::DeviceIntegration *m_integration = nullptr;
};

EglFSDeviceIntegration::EglFSDeviceIntegration()
{
    auto integrationKeys = Aurora::Platform::DeviceIntegrationFactory::keys();
    if (!integrationKeys.isEmpty()) {
        // Prioritize either Wayland or KMS/DRM
        if (qEnvironmentVariableIsSet("WAYLAND_DISPLAY")) {
            const auto waylandKey = QStringLiteral("wayland");
            if (integrationKeys.contains(waylandKey)) {
                integrationKeys.removeOne(waylandKey);
                integrationKeys.prepend(waylandKey);
            }
        } else {
            const auto drmKey = QStringLiteral("drm");
            if (integrationKeys.contains(drmKey)) {
                integrationKeys.removeOne(drmKey);
                integrationKeys.prepend(drmKey);
            }

            const auto waylandKey = QStringLiteral("wayland");
            if (integrationKeys.contains(waylandKey))
                integrationKeys.removeOne(waylandKey);
        }

        QByteArray requested;

        // Override with an environment variable
        if (qEnvironmentVariableIsSet("AURORA_QPA_INTEGRATION"))
            requested = qgetenv("AURORA_QPA_INTEGRATION");

        if (!requested.isEmpty()) {
            const auto requestedString = QString::fromLocal8Bit(requested);
            integrationKeys.removeOne(requestedString);
            integrationKeys.prepend(requestedString);
        }

        qCDebug(gLcEglFS) << "Device integration plugin keys:" << integrationKeys;
        while (!m_integration && !integrationKeys.isEmpty()) {
            const auto key = integrationKeys.takeFirst();
            qCDebug(gLcEglFS) << "Trying to load device integration:" << key;
            m_integration = Aurora::Platform::DeviceIntegrationFactory::create(key);
            if (m_integration)
                qCInfo(gLcEglFS) << "Loaded device integration" << key;
        }
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    if (!m_integration)
        qCFatal(gLcEglFS, "No suitable device integration found!");
#else
    if (!m_integration)
        qFatal("No suitable device integration found!");
#endif
}

EglFSDeviceIntegration::~EglFSDeviceIntegration()
{
    if (m_integration) {
        m_integration->deleteLater();
        m_integration = nullptr;
    }
}

Aurora::Platform::DeviceIntegration *EglFSDeviceIntegration::deviceIntegration() const
{
    return m_integration;
}

Q_GLOBAL_STATIC(EglFSDeviceIntegration, eglfsDeviceIntegration)

Aurora::Platform::DeviceIntegration *auroraDeviceIntegration()
{
    return eglfsDeviceIntegration()->deviceIntegration();
}

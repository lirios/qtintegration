# SPDX-FileCopyrightText: 2023 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

## Find Wayland:
find_package(Wayland 1.15 COMPONENTS Client)
set_package_properties(Wayland PROPERTIES TYPE REQUIRED)

## Find wayland-protocols:
find_package(WaylandProtocols 1.15)
set_package_properties(WaylandProtocols PROPERTIES TYPE REQUIRED)

## Find Qt 5:
set(QT_MIN_VERSION "5.15.0")
find_package(Qt5 "${QT_MIN_VERSION}"
    CONFIG REQUIRED
    COMPONENTS
        Core
        DBus
        Gui
        Widgets
        QuickControls2
        WaylandClient
)

#### Features

# Decorations: material
option(FEATURE_qtintegration_material_decoration "Material decoration" ON)
add_feature_info("QtIntegration::MaterialDecoration" FEATURE_qtintegration_material_decoration "Build Material decoration")
set(LIRI_FEATURE_qtintegration_material_decoration "$<IF:${FEATURE_qtintegration_material_decoration},1,0>")

# Qt platform theme
option(FEATURE_qtintegration_platformtheme "Qt platform theme" ON)
add_feature_info("QtIntegration::PlatformTheme" FEATURE_qtintegration_platformtheme "Build Qt platform theme")
set(LIRI_FEATURE_qtintegration_platformtheme "$<IF:${FEATURE_qtintegration_platformtheme},1,0>")

# Qt platforms
option(FEATURE_qtintegration_aurora_eglfs "Qt platform plugin for Aurora compositors" ON)
if(FEATURE_qtintegration_aurora_eglfs)
    find_package(Qt${QT_MAJOR_VERSION}FontDatabaseSupport QUIET)
    find_package(Qt${QT_MAJOR_VERSION}ThemeSupport QUIET)
    find_package(Qt${QT_MAJOR_VERSION}EventDispatcherSupport QUIET)
    find_package(Qt${QT_MAJOR_VERSION}EglSupport QUIET)
    find_package(Qt${QT_MAJOR_VERSION}PlatformCompositorSupport QUIET)
    find_package(Qt${QT_MAJOR_VERSION}ServiceSupport QUIET)
    find_package(Qt${QT_MAJOR_VERSION}FbSupport QUIET)
    find_package(EGL QUIET)
    find_package(Fontconfig QUIET)
    find_package(Liri1AuroraPlatform QUIET)

    if(NOT TARGET Qt${QT_MAJOR_VERSION}FontDatabaseSupport::Qt${QT_MAJOR_VERSION}FontDatabaseSupport)
        message(WARNING "You need Qt${QT_MAJOR_VERSION}FontDatabaseSupport for QtIntegration::AuroraEglFS")
        set(FEATURE_qtintegration_aurora_eglfs OFF)
    endif()
    if(NOT TARGET Qt${QT_MAJOR_VERSION}ThemeSupport::Qt${QT_MAJOR_VERSION}ThemeSupport)
        message(WARNING "You need Qt${QT_MAJOR_VERSION}ThemeSupport for QtIntegration::AuroraEglFS")
        set(FEATURE_qtintegration_aurora_eglfs OFF)
    endif()
    if(NOT TARGET Qt${QT_MAJOR_VERSION}EventDispatcherSupport::Qt${QT_MAJOR_VERSION}EventDispatcherSupport)
        message(WARNING "You need Qt${QT_MAJOR_VERSION}EventDispatcherSupport for QtIntegration::AuroraEglFS")
        set(FEATURE_qtintegration_aurora_eglfs OFF)
    endif()
    if(NOT TARGET Qt${QT_MAJOR_VERSION}EglSupport::Qt${QT_MAJOR_VERSION}EglSupport)
        message(WARNING "You need Qt${QT_MAJOR_VERSION}EglSupport for QtIntegration::AuroraEglFS")
        set(FEATURE_qtintegration_aurora_eglfs OFF)
    endif()
    if(NOT TARGET Qt${QT_MAJOR_VERSION}PlatformCompositorSupport::Qt${QT_MAJOR_VERSION}PlatformCompositorSupport)
        message(WARNING "You need Qt${QT_MAJOR_VERSION}PlatformCompositorSupport for QtIntegration::AuroraEglFS")
        set(FEATURE_qtintegration_aurora_eglfs OFF)
    endif()
    if(NOT TARGET Qt${QT_MAJOR_VERSION}ServiceSupport::Qt${QT_MAJOR_VERSION}ServiceSupport)
        message(WARNING "You need Qt${QT_MAJOR_VERSION}ServiceSupport for QtIntegration::AuroraEglFS")
        set(FEATURE_qtintegration_aurora_eglfs OFF)
    endif()
    if(NOT TARGET Qt${QT_MAJOR_VERSION}FbSupport::Qt${QT_MAJOR_VERSION}FbSupport)
        message(WARNING "You need Qt${QT_MAJOR_VERSION}FbSupport for QtIntegration::AuroraEglFS")
        set(FEATURE_qtintegration_aurora_eglfs OFF)
    endif()
    if(NOT TARGET EGL::EGL)
        message(WARNING "You need EGL for QtIntegration::AuroraEglFS")
        set(FEATURE_qtintegration_aurora_eglfs OFF)
    endif()
    if(NOT TARGET Fontconfig::Fontconfig)
        message(WARNING "You need Fontconfig for QtIntegration::AuroraEglFS")
        set(FEATURE_qtintegration_aurora_eglfs OFF)
    endif()
    if(NOT TARGET Liri::AuroraPlatform)
        message(WARNING "You need AuroraPlatform for QtIntegration::AuroraEglFS")
        set(FEATURE_qtintegration_aurora_eglfs OFF)
    endif()
endif()
add_feature_info("QtIntegration::AuroraEglFS" FEATURE_qtintegration_aurora_eglfs "Build Qt platform plugin for Aurora compositors")
set(LIRI_FEATURE_qtintegration_aurora_eglfs "$<IF:${FEATURE_qtintegration_aurora_eglfs},1,0>")

# QtWayland shell integrations: layer-shell
option(FEATURE_qtintegration_layer_shell_integration "Layer-shell QtWayland shell integration" ON)
add_feature_info("QtIntegration::LayerShell" FEATURE_qtintegration_layer_shell_integration "Build layer-shell QtWayland shell integration")
set(LIRI_FEATURE_qtintegration_layer_shell_integration "$<IF:${FEATURE_qtintegration_layer_shell_integration},1,0>")

# QtWayland shell integrations: lockscreen
option(FEATURE_qtintegration_lockscreen_integration "Lockscreen QtWayland shell integration" ON)
add_feature_info("QtIntegration::LockScreen" FEATURE_qtintegration_lockscreen_integration "Build lockscreen QtWayland shell integration")
set(LIRI_FEATURE_qtintegration_lockscreen_integration "$<IF:${FEATURE_qtintegration_lockscreen_integration},1,0>")

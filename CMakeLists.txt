cmake_minimum_required(VERSION 3.10.0)

project("PlatformIntegration"
    VERSION "1.0.0"
    DESCRIPTION "Integration of Qt applications with the Liri desktop environment"
    LANGUAGES CXX C
)

## Shared macros and functions:
if(NOT LIRI_LOCAL_ECM)
    find_package(LiriCMakeShared "2.0.0" REQUIRED NO_MODULE)
    list(APPEND CMAKE_MODULE_PATH "${LCS_MODULE_PATH}")
endif()

## Add some paths to check for CMake modules:
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_CURRENT_SOURCE_DIR}/cmake/3rdparty/extra-cmake-modules")

## Set minimum versions required:
set(QT_MIN_VERSION "5.15.0")

## Liri specific setup common for all modules:
include(LiriSetup)

## Only build on the appropriate platforms.
if(NOT UNIX OR ANDROID)
    message(NOTICE "Skipping the build as the condition \"UNIX OR ANDROID\" is not met.")
    return()
endif()

## Features:
option(FEATURE_qtintegration_material_decoration "Material decoration" ON)
add_feature_info("QtIntegration::MaterialDecoration" LIRI_QTINTEGRATION_ENABLE_MATERIAL_DECORATION "Build Material decoration")
option(FEATURE_qtintegration_platformtheme "Qt platform theme" ON)
add_feature_info("QtIntegration::PlatformTheme" LIRI_QTINTEGRATION_ENABLE_PLATFORMTHEME "Build Qt platform theme")
option(FEATURE_qtintegration_layer_shell_integration "Layer-shell QtWayland shell integration" ON)
add_feature_info("QtIntegration::LayerShell" LIRI_QTINTEGRATION_ENABLE_LAYER_SHELL_INTEGRATION "Build layer-shell QtWayland shell integration")
option(FEATURE_qtintegration_lockscreen_integration "Lockscreen QtWayland shell integration" ON)
add_feature_info("QtIntegration::LockScreen" LIRI_QTINTEGRATION_ENABLE_LOCKSCREEN_INTEGRATION "Build lockscreen QtWayland shell integration")

## Find Wayland:
find_package(Wayland 1.15 COMPONENTS Client)
set_package_properties(Wayland PROPERTIES TYPE REQUIRED)

## Find wayland-protocols:
find_package(WaylandProtocols 1.15)
set_package_properties(WaylandProtocols PROPERTIES TYPE REQUIRED)

## Find Qt 5:
find_package(Qt5 "${QT_MIN_VERSION}"
    CONFIG REQUIRED
    COMPONENTS
        Core
        Gui
        Widgets
        QuickControls2
        WaylandClient
)

## Add subdirectories:
if(FEATURE_qtintegration_material_decoration)
    add_subdirectory(src/plugins/decorations/material)
endif()
if(FEATURE_qtintegration_platformtheme)
    add_subdirectory(src/plugins/platformtheme)
endif()
if(FEATURE_qtintegration_layer_shell_integration)
    add_subdirectory(src/plugins/shellintegration/layershell)
endif()
if(FEATURE_qtintegration_lockscreen_integration)
    add_subdirectory(src/plugins/shellintegration/lockscreen)
endif()

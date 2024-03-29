cmake_minimum_required(VERSION 3.19)

project("QtIntegration"
    VERSION "1.0.0"
    DESCRIPTION "Integration of Qt applications with the Liri desktop environment"
    LANGUAGES CXX C
)

## Only build on the appropriate platforms.
if(NOT (UNIX OR NOT CMAKE_SYSTEM_NAME MATCHES "Android"))
    message(NOTICE "Skipping the build as the condition \"UNIX OR NOT Android\" is not met.")
    return()
endif()

## ECM:
find_package(ECM 5.245.0 REQUIRED NO_MODULE)
list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

## Installation directories:
include(KDEInstallDirs)

## Compiler settings:
set(KDE_SKIP_NULLPTR_WARNINGS_SETTINGS TRUE)
include(KDECompilerSettings NO_POLICY_SCOPE)

## Enable keywords such as emit:
# TODO: Replace keywords such as emit with Q_EMIT
remove_definitions(-DQT_NO_KEYWORDS)

## CMake settings:
include(KDECMakeSettings)

## QML module:
include(ECMQmlModule)

## ECM:
include(ECMQtDeclareLoggingCategory)

## Disable use of C++ API deprecated in Qt 5.15
add_compile_definitions(QT_DISABLE_DEPRECATED_UP_TO=0x050F00)

## Features:
include(features.cmake)

## Add subdirectories:
if(FEATURE_qtintegration_material_decoration)
    add_subdirectory(src/plugins/decorations/material)
endif()
if(FEATURE_qtintegration_platformtheme)
    add_subdirectory(src/plugins/platformtheme)
endif()

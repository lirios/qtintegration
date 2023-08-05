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
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_CURRENT_SOURCE_DIR}/cmake/3rdparty/extra-cmake-modules;${CMAKE_CURRENT_SOURCE_DIR}/cmake/3rdparty/kwin")

## Liri specific setup common for all modules:
include(LiriSetup)

## Only build on the appropriate platforms.
if(NOT UNIX OR ANDROID)
    message(NOTICE "Skipping the build as the condition \"UNIX OR ANDROID\" is not met.")
    return()
endif()

## Features:
include(features.cmake)
if(NOT LIRI_SUPERBUILD)
    feature_summary(WHAT ENABLED_FEATURES DISABLED_FEATURES)
endif()

## Add subdirectories:
if(FEATURE_qtintegration_material_decoration)
    add_subdirectory(src/plugins/decorations/material)
endif()
if(FEATURE_qtintegration_platformtheme)
    add_subdirectory(src/plugins/platformtheme)
endif()
if(FEATURE_qtintegration_aurora_eglfs)
    add_subdirectory(src/plugins/platforms/eglfs)
endif()
if(FEATURE_qtintegration_layer_shell_integration)
    add_subdirectory(src/plugins/shellintegration/layershell)
endif()
if(FEATURE_qtintegration_lockscreen_integration)
    add_subdirectory(src/plugins/shellintegration/lockscreen)
endif()

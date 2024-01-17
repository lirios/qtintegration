# SPDX-FileCopyrightText: 2023 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

## Find Qt:
find_package(Qt6 "6.6.0"
    REQUIRED
    COMPONENTS
        Core
        Gui
        Widgets
        QuickControls2
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

## Features summary:
if(NOT LIRI_SUPERBUILD)
    feature_summary(WHAT ENABLED_FEATURES DISABLED_FEATURES)
endif()

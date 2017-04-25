!exists(features/liri_deployment.prf): \
    error("Git submodule missing. Run \'git submodule update --init\' in $${PWD}.")

TARGET = liritheme

PLUGIN_TYPE = platformthemes
PLUGIN_EXTENDS = -
PLUGIN_CLASS_NAME = LiriThemePlugin
load(liri_qt_plugin)

QT += gui widgets quickcontrols2 gui-private
CONFIG += link_pkgconfig no_install_prl
PKGCONFIG += Qt5GSettings

HEADERS += \
    $$PWD/src/hintssettings.h \
    $$PWD/src/liritheme.h \
    $$PWD/src/liritheme_p.h \
    $$PWD/src/resourcehelper.h

SOURCES += \
    $$PWD/src/hintssettings.cpp \
    $$PWD/src/logging_p.cpp \
    $$PWD/src/liritheme.cpp \
    $$PWD/src/liritheme_p.cpp \
    $$PWD/src/main.cpp \
    $$PWD/src/resourcehelper.cpp

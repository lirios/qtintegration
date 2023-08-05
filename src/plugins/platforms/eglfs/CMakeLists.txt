include(ECMQtDeclareLoggingCategory)
ecm_qt_declare_logging_category(
    AuroraEglFS_SOURCES
    HEADER "eglfscategories.h"
    IDENTIFIER "gLcEglFS"
    CATEGORY_NAME "aurora.eglfs"
    DEFAULT_SEVERITY "Info"
    DESCRIPTION "Aurora EGLFS Qt platform plugin"
)

liri_add_plugin(AuroraEglFSPlatformPlugin
    TYPE
        platforms
    OUTPUT_NAME
        aurora-eglfs-next
    SOURCES
        eglfsconfigchooser.cpp eglfsconfigchooser.h
        eglfscontext.cpp eglfscontext.h
        eglfscursor.cpp eglfscursor.h
        eglfsdeviceintegration.cpp eglfsdeviceintegration.h
        eglfsinfo.cpp eglfsinfo.h
        eglfsinputmanager.cpp eglfsinputmanager.h
        eglfsintegration.cpp eglfsintegration.h
        eglfsmain.cpp
        eglfsscreen.cpp eglfsscreen.h
        eglfswindow.cpp eglfswindow.h
        aurora-eglfs.json
        ${AuroraEglFS_SOURCES}
    PUBLIC_DEFINES
        QT_EGL_NO_X11
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
    PUBLIC_LIBRARIES
        Qt5::Core
        Qt5::CorePrivate
        Qt5::DBus
        Qt5::GuiPrivate
        Qt5::Gui
        Fontconfig::Fontconfig
        EGL::EGL
        Liri::AuroraPlatform
    LIBRARIES
        Qt5FontDatabaseSupport::Qt5FontDatabaseSupport
        Qt5ThemeSupport::Qt5ThemeSupport
        Qt5EventDispatcherSupport::Qt5EventDispatcherSupport
        Qt5EglSupport::Qt5EglSupport
        Qt5PlatformCompositorSupport::Qt5PlatformCompositorSupport
        Qt5ServiceSupport::Qt5ServiceSupport
        Qt5FbSupport::Qt5FbSupport
)

liri_finalize_plugin(AuroraEglFSPlatformPlugin)

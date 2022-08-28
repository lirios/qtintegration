find_package(Xkbcommon REQUIRED)
find_package(Qt5XkbCommonSupport REQUIRED PRIVATE)

if(NOT TARGET Liri::AuroraClient)
    find_package(Liri1AuroraClient REQUIRED)
endif()

liri_add_plugin(liri-lockscreen
    TYPE
        wayland-shell-integration
    SOURCES
        qwaylandlockscreenintegration.cpp qwaylandlockscreenintegration_p.h
        qwaylandlockscreenintegrationplugin.cpp
        qwaylandextsessionlock.cpp qwaylandextsessionlock_p.h
    LIBRARIES
        Qt5::WaylandClientPrivate
        Qt5::XkbCommonSupport
        Qt5::XkbCommonSupportPrivate
        PkgConfig::Xkbcommon
        Liri::AuroraClient
)

liri_generate_wayland_protocol_client_sources(liri-lockscreen
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/ext-session-lock-v1.xml
)

liri_finalize_plugin(liri-lockscreen)

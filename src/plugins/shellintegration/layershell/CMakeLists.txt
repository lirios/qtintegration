find_package(XkbCommon REQUIRED)
find_package(Qt5XkbCommonSupport REQUIRED PRIVATE)

if(NOT TARGET Liri::AuroraClient)
    find_package(Liri1AuroraClient REQUIRED)
endif()

liri_add_plugin(liri-layer-shell
    TYPE
        wayland-shell-integration
    SOURCES
        qwaylandlayershell.cpp
        qwaylandlayershell_p.h
        qwaylandlayershellintegration.cpp
        qwaylandlayershellintegration_p.h
        qwaylandlayershellintegrationplugin.cpp
        qwaylandlayersurface.cpp
        qwaylandlayersurface_p.h
        qwaylandxdgshell.cpp
        qwaylandxdgshell_p.h
        qwaylandxdgsurface.cpp
        qwaylandxdgsurface_p.h
    LIBRARIES
        Qt5::WaylandClientPrivate
        Qt5::XkbCommonSupport
        Qt5::XkbCommonSupportPrivate
        XkbCommon::XkbCommon
        Liri::AuroraClient
)

liri_generate_wayland_protocol_client_sources(liri-layer-shell
    FILES
        ${WaylandProtocols_DATADIR}/stable/xdg-shell/xdg-shell.xml
        ${CMAKE_CURRENT_SOURCE_DIR}/wlr-layer-shell-unstable-v1.xml
)

liri_finalize_plugin(liri-layer-shell)

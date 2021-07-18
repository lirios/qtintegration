find_package(Xkbcommon REQUIRED)
find_package(Qt5XkbCommonSupport REQUIRED PRIVATE)
find_package(QtWaylandScanner REQUIRED)

if(NOT TARGET Liri::WaylandClientLayerShell)
    find_package(Liri1WaylandClientLayerShell REQUIRED)
endif()

ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${WaylandProtocols_DATADIR}/stable/xdg-shell/xdg-shell.xml"
    BASENAME "xdg-shell")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/wlr-layer-shell-unstable-v1.xml"
    BASENAME "wlr-layer-shell-unstable-v1")

set(_generated_c_protocol_files
    "${CMAKE_CURRENT_BINARY_DIR}/wayland-wlr-layer-shell-unstable-v1-client-protocol.h"
    "${CMAKE_CURRENT_BINARY_DIR}/wayland-xdg-shell-client-protocol.h"
)
set(_generated_cpp_protocol_files
    "${CMAKE_CURRENT_BINARY_DIR}/qwayland-wlr-layer-shell-unstable-v1.h"
    "${CMAKE_CURRENT_BINARY_DIR}/qwayland-xdg-shell.h"
)
set_source_files_properties(${_generated_c_protocol_files} PROPERTIES SKIP_AUTOMOC ON)

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
        ${SOURCES}
        ${_generated_c_protocol_files}
        ${_generated_cpp_protocol_files}
    LIBRARIES
        Qt5::Gui
        Qt5::WaylandClient
        Qt5::WaylandClientPrivate
        Qt5::XkbCommonSupportPrivate
        Wayland::Client
        PkgConfig::Xkbcommon
        Liri::WaylandClientLayerShell
)

find_package(Wayland REQUIRED)
find_package(Xkbcommon REQUIRED)
find_package(Qt5XkbCommonSupport REQUIRED PRIVATE)

liri_add_plugin(materialdecoration
    TYPE
        wayland-decoration-client
    SOURCES
        materialdecoration.cpp
        materialdecoration.h
        plugin.cpp
    LIBRARIES
        Qt5::XkbCommonSupport
        Qt5::XkbCommonSupportPrivate
        Qt5::WaylandClient
        Qt5::WaylandClientPrivate
        Wayland::Client
        PkgConfig::Xkbcommon
)

liri_finalize_plugin(materialdecoration)

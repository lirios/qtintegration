find_package(Wayland REQUIRED)
find_package(XkbCommon REQUIRED)
#find_package(Qt6XkbCommonSupport REQUIRED PRIVATE)

qt6_add_plugin(MaterialQtWaylandDecoration
    SHARED
    CLASS_NAME QWaylandMaterialDecorationPlugin
    MANUAL_FINALIZATION
    materialdecoration.cpp materialdecoration.h
    plugin.cpp
)

set_target_properties(MaterialQtWaylandDecoration
    PROPERTIES OUTPUT_NAME liri-material
)

target_link_libraries(MaterialQtWaylandDecoration
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::WaylandClient
    PRIVATE
        Wayland::Client
        XkbCommon::XkbCommon
        #Qt6::XkbCommonSupport
        #Qt6::XkbCommonSupportPrivate
        Qt6::WaylandClientPrivate
)

qt6_finalize_target(MaterialQtWaylandDecoration)

install(
    TARGETS MaterialQtWaylandDecoration
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/wayland-decoration-client
)

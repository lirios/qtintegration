if(NOT TARGET Liri::Qt6GSettings)
    find_package(Qt6GSettings REQUIRED)
endif()

qt6_add_plugin(LiriQtPlatformThemePlugin
    SHARED
    CLASS_NAME LiriThemePlugin
    MANUAL_FINALIZATION
    hintssettings.cpp hintssettings.h
    liritheme.cpp liritheme.h
    logging_p.cpp logging_p.h
    main.cpp
    resourcehelper.cpp resourcehelper.h
)

set_target_properties(LiriQtPlatformThemePlugin
    PROPERTIES OUTPUT_NAME liritheme
)

target_link_libraries(LiriQtPlatformThemePlugin
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::QuickControls2
    PRIVATE
        Qt6::GuiPrivate
        Liri::Qt6GSettings
)

qt6_finalize_target(LiriQtPlatformThemePlugin)

install(
    TARGETS LiriQtPlatformThemePlugin
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/platformthemes
)

if(NOT TARGET Liri::Qt5GSettings)
    find_package(Qt5GSettings REQUIRED)
endif()

liri_add_plugin(liritheme
    TYPE
        platformthemes
    SOURCES
        hintssettings.cpp
        hintssettings.h
        liritheme.cpp
        liritheme.h
        logging_p.cpp
        logging_p.h
        main.cpp
        resourcehelper.cpp
        resourcehelper.h
    LIBRARIES
        Qt5::GuiPrivate
        Qt5::Widgets
        Qt5::QuickControls2
        Liri::Qt5GSettings
)

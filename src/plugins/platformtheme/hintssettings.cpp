// Copyright (C) 2010-2016 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtCore/QDir>
#include <QtCore/QEvent>
#include <QtCore/QFileInfo>
#include <QtGui/QTextCharFormat>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QStandardPaths>

#include <Qt6GSettings/QGSettings>

#include "hintssettings.h"

using namespace Qt::Literals::StringLiterals;

// Default fonts
static const char defaultSystemFontName[] = "Droid Sans";
static int defaultSystemFontSize = 11;
static const char defaultMonospaceFontName[] = "Droid Sans Mono";
static int defaultMonospaceFontSize = 11;

/*!
 * Returns a list of XDG compliant paths for icon themes.
 * See https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
 */
static QStringList xdgIconThemePaths()
{
    QStringList paths;
    QStringList xdgPaths;

    // Add ~/.icons for backwards compatibility
    paths.append(QDir::home().absoluteFilePath(QLatin1String(".icons")));

    // Add XDG_DATA_HOME
    QString xdgDataHome = QFile::decodeName(qgetenv("XDG_DATA_HOME"));
    if (xdgDataHome.isEmpty())
        xdgDataHome = QDir::home().absoluteFilePath(QLatin1String(".local/share"));
    xdgPaths.append(xdgDataHome);

    // Add XDG_DATA_DIRS
    QString xdgDataDirs = QFile::decodeName(qgetenv("XDG_DATA_DIRS"));
    if (xdgDataDirs.isEmpty())
        xdgDataDirs = QLatin1String("/usr/local/share/:/usr/share/");

    // Split ':' path separator from XDG data directories
    xdgPaths += xdgDataDirs.split(QLatin1Char(':'), Qt::SkipEmptyParts);

    // Append the icons directory to all XDG data directories
    for (const QString &xdgPath : qAsConst(xdgPaths)) {
        QDir iconDir = QDir(xdgPath).filePath(QLatin1String("icons"));
        paths.append(iconDir.absolutePath());
    }

    // Remove duplicates
    paths.removeDuplicates();

    return paths;
}

static QFont *readFont(const QString &family, int size)
{
    QFont font(family, size);
    return new QFont(font);
}

HintsSettings::HintsSettings(QObject *parent)
    : QObject(parent)
{
    m_settings = new QtGSettings::QGSettings(QStringLiteral("io.liri.desktop.interface"),
                                             QStringLiteral("/io/liri/desktop/interface/"));

    // Change only the few hints involved, for some of these settings
    // we need to take actions to refresh applications
    connect(m_settings, &QtGSettings::QGSettings::settingChanged, this, [this](const QString &key) {
        if (key == QStringLiteral("cursorBlinkTime"))
            qtSettingsChanged();
        else if (key == QStringLiteral("toolButtonStyle"))
            toolButtonStyleChanged();
        else if (key == QStringLiteral("iconTheme") || key == QStringLiteral("toolbarIconsSize"))
            iconChanged();
        else if (key == QStringLiteral("widgetsStyle"))
            styleChanged();
        else if (key == QStringLiteral("colorScheme"))
            paletteChanged();
        else if (key.toLower().contains(QStringLiteral("font")))
            fontChanged();
    });
}

HintsSettings::~HintsSettings()
{
    m_settings->deleteLater();
}

void HintsSettings::refresh()
{
    m_resources.clear();

    // Load hints
    collectHints();

    // Palette
    refreshPalette();

    // Fonts
    refreshFonts();
}

void HintsSettings::collectHints()
{
    m_hints.clear();

    m_hints.insert(QPlatformTheme::CursorFlashTime,
                   m_settings->value(QStringLiteral("cursorBlinkTime")));
    m_hints.insert(QPlatformTheme::DropShadow, true);
    m_hints.insert(QPlatformTheme::ToolButtonStyle,
                   toolButtonStyle(m_settings->value(QStringLiteral("toolButtonStyle"))));
    m_hints.insert(QPlatformTheme::ToolBarIconSize,
                   toolBarIconSize(m_settings->value(QStringLiteral("toolbarIconsSize"))));
    m_hints.insert(QPlatformTheme::ItemViewActivateItemOnSingleClick, false);
    m_hints.insert(QPlatformTheme::SystemIconThemeName,
                   m_settings->value(QStringLiteral("iconTheme")));
    m_hints.insert(QPlatformTheme::SystemIconFallbackThemeName, QStringLiteral("hicolor"));
    m_hints.insert(QPlatformTheme::IconThemeSearchPaths, xdgIconThemePaths());
    m_hints.insert(QPlatformTheme::StyleNames,
                   QStringList() << m_settings->value(QStringLiteral("widgetsStyle")).toString());
    m_hints.insert(QPlatformTheme::WindowAutoPlacement, true);
    m_hints.insert(QPlatformTheme::DialogButtonBoxLayout, QDialogButtonBox::MacLayout);
    m_hints.insert(QPlatformTheme::DialogButtonBoxButtonsHaveIcons, false);
    m_hints.insert(QPlatformTheme::UseFullScreenForPopupMenu, true);
    // TODO: Use the Mac keyboard scheme only if an Apple keyboard is detected
    // int(MacKeyboardScheme);
    m_hints.insert(QPlatformTheme::KeyboardScheme, int(QPlatformTheme::GnomeKeyboardScheme));
    m_hints.insert(
            QPlatformTheme::UiEffects,
            QPlatformTheme::AnimateMenuUiEffect | QPlatformTheme::FadeMenuUiEffect
                    | QPlatformTheme::AnimateComboUiEffect | QPlatformTheme::AnimateTooltipUiEffect
                    | QPlatformTheme::FadeTooltipUiEffect | QPlatformTheme::AnimateToolBoxUiEffect);
    m_hints.insert(QPlatformTheme::SpellCheckUnderlineStyle,
                   int(QTextCharFormat::SpellCheckUnderline));
    m_hints.insert(QPlatformTheme::TabFocusBehavior, int(Qt::TabFocusAllControls));
    m_hints.insert(QPlatformTheme::MouseCursorTheme, m_settings->value("cursorTheme"_L1));
    m_hints.insert(QPlatformTheme::MouseCursorSize, m_settings->value("cursorSize"_L1));
    QList<int> pixmapSizes;
    pixmapSizes << 512 << 256 << 128 << 96 << 64 << 48 << 32 << 24 << 22 << 16;
    m_hints.insert(QPlatformTheme::IconPixmapSizes, QVariant::fromValue(pixmapSizes));
}

void HintsSettings::refreshPalette()
{
    // Locate color scheme
    QString scheme = m_settings->value(QStringLiteral("colorScheme")).toString();
    QString schemeFileName =
            QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                   QStringLiteral("color-schemes/%1.colors").arg(scheme));

    // Palette
    QPalette systemPalette = QPalette();
    ResourceHelper::readPalette(schemeFileName, &systemPalette);
    m_resources.palettes[QPlatformTheme::SystemPalette] = new QPalette(systemPalette);
}

void HintsSettings::refreshFonts()
{
    // Fonts
    QString fontFamily = m_settings->value(QStringLiteral("fontName")).toString();
    int fontSize = m_settings->value(QStringLiteral("fontSize")).toInt();
    QString monospaceFontFamily = m_settings->value(QStringLiteral("monospaceFontName")).toString();
    int monospaceFontSize = m_settings->value(QStringLiteral("monospaceFontSize")).toInt();
    QString smallFontFamily = m_settings->value(QStringLiteral("smallFontName")).toString();
    int smallFontSize = m_settings->value(QStringLiteral("smallFontSize")).toInt();
    QString miniFontFamily = m_settings->value(QStringLiteral("miniFontName")).toString();
    int miniFontSize = m_settings->value(QStringLiteral("miniFontSize")).toInt();

    // System font
    if (QFont *systemFont = readFont(fontFamily, fontSize))
        m_resources.fonts[QPlatformTheme::SystemFont] = systemFont;
    else
        m_resources.fonts[QPlatformTheme::SystemFont] =
                new QFont(QLatin1String(defaultSystemFontName), defaultSystemFontSize);

    // Monospace font
    if (QFont *monospaceFont = readFont(monospaceFontFamily, monospaceFontSize))
        m_resources.fonts[QPlatformTheme::FixedFont] = monospaceFont;
    else
        m_resources.fonts[QPlatformTheme::FixedFont] =
                new QFont(QLatin1String(defaultMonospaceFontName), defaultMonospaceFontSize);

    // Small font
    if (QFont *smallFont = readFont(smallFontFamily, smallFontSize))
        m_resources.fonts[QPlatformTheme::SmallFont] = smallFont;
    else
        m_resources.fonts[QPlatformTheme::SmallFont] =
                new QFont(QLatin1String(defaultSystemFontName), defaultSystemFontSize);

    // Mini font
    if (QFont *miniFont = readFont(miniFontFamily, miniFontSize))
        m_resources.fonts[QPlatformTheme::MiniFont] = miniFont;
    else
        m_resources.fonts[QPlatformTheme::MiniFont] =
                new QFont(QLatin1String(defaultSystemFontName), defaultSystemFontSize);

    // Other fonts
    QList<QPlatformTheme::Font> fonts;
    fonts << QPlatformTheme::TitleBarFont << QPlatformTheme::MdiSubWindowTitleFont
          << QPlatformTheme::DockWidgetTitleFont;
    for (QPlatformTheme::Font font : qAsConst(fonts)) {
        if (QFont *systemFont = readFont(fontFamily, fontSize))
            m_resources.fonts[font] = systemFont;
        else
            m_resources.fonts[font] =
                    new QFont(QLatin1String(defaultSystemFontName), defaultSystemFontSize);
        m_resources.fonts[font]->setBold(true);
    }
}

Qt::ToolButtonStyle HintsSettings::toolButtonStyle(const QVariant &value)
{
    const QString style = value.toString();

    if (style == QStringLiteral("icon-only"))
        return Qt::ToolButtonIconOnly;
    else if (style == QStringLiteral("text-only"))
        return Qt::ToolButtonTextOnly;
    else if (style == QStringLiteral("text-beside-icon"))
        return Qt::ToolButtonTextBesideIcon;
    else if (style == QStringLiteral("text-under-icon"))
        return Qt::ToolButtonTextUnderIcon;

    return Qt::ToolButtonFollowStyle;
}

int HintsSettings::toolBarIconSize(const QVariant &value)
{
    const QString size = value.toString();

    if (size == QStringLiteral("small"))
        return 24;
    return 48;
}

void HintsSettings::qtSettingsChanged()
{
    // Change the hints
    m_hints.insert(QPlatformTheme::CursorFlashTime,
                   m_settings->value(QStringLiteral("cursorBlinkTime")));
}

void HintsSettings::toolButtonStyleChanged()
{
    // Change the hint
    m_hints.insert(QPlatformTheme::ToolButtonStyle,
                   toolButtonStyle(m_settings->value(QStringLiteral("toolButtonStyle"))));

    // Continue only for QWidgets applications
    if (!qobject_cast<QApplication *>(QCoreApplication::instance()))
        return;

    // Refresh all tool button widgets
    QWidgetList widgets = QApplication::allWidgets();
    for (QWidget *widget : qAsConst(widgets)) {
        if (qobject_cast<QToolButton *>(widget)) {
            QEvent event(QEvent::StyleChange);
            QApplication::sendEvent(widget, &event);
        }
    }
}

void HintsSettings::iconChanged()
{
    // Change the hint
    m_hints.insert(QPlatformTheme::SystemIconThemeName,
                   m_settings->value(QStringLiteral("iconTheme")));

    // This method might be called when only the icon theme has changed,
    // in that case we don't refresh the style
    int newSize = toolBarIconSize(m_settings->value(QStringLiteral("toolbarIconsSize")));
    if (m_hints[QPlatformTheme::ToolBarIconSize] != newSize) {
        m_hints.insert(QPlatformTheme::ToolBarIconSize, newSize);

        // Continue only for QWidgets applications
        if (!qobject_cast<QApplication *>(QCoreApplication::instance()))
            return;

        // Refresh the main window and all tool bars
        QWidgetList widgets = QApplication::allWidgets();
        for (QWidget *widget : qAsConst(widgets)) {
            if (qobject_cast<QToolBar *>(widget) || qobject_cast<QMainWindow *>(widget)) {
                QEvent event(QEvent::StyleChange);
                QApplication::sendEvent(widget, &event);
            }
        }
    }
}

void HintsSettings::styleChanged()
{
    // Change the hint
    m_hints.insert(QPlatformTheme::StyleNames,
                   QStringList() << m_settings->value(QStringLiteral("widgetsStyle")).toString());

    // Continue only for QWidgets applications
    if (!qobject_cast<QApplication *>(QCoreApplication::instance()))
        return;

    // Change the style
    const QString style = m_hints[QPlatformTheme::StyleNames].toString();
    if (!style.isEmpty())
        QApplication::setStyle(style);
}

void HintsSettings::paletteChanged()
{
    refreshPalette();

    // Change palette
    if (qobject_cast<QApplication *>(QCoreApplication::instance())) {
        QPalette palette = *m_resources.palettes[QPlatformTheme::SystemPalette];
        QApplication::setPalette(palette);
        Q_EMIT qApp->paletteChanged(palette);
    } else if (qobject_cast<QGuiApplication *>(QCoreApplication::instance())) {
        QPalette palette = *m_resources.palettes[QPlatformTheme::SystemPalette];
        QGuiApplication::setPalette(palette);
    }
}

void HintsSettings::fontChanged()
{
    refreshFonts();

    // Change system font
    if (qobject_cast<QApplication *>(QCoreApplication::instance()))
        QApplication::setFont(*m_resources.fonts[QPlatformTheme::SystemFont]);
    else if (qobject_cast<QGuiApplication *>(QCoreApplication::instance()))
        QGuiApplication::setFont(*m_resources.fonts[QPlatformTheme::SystemFont]);
}

#include "moc_hintssettings.cpp"

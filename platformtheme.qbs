import qbs 1.0

Project {
    name: "PlatformTheme"

    readonly property string version: "1.0.0"

    readonly property string minimumQtVersion: "5.8.0"

    property bool useStaticAnalyzer: false

    condition: qbs.targetOS.contains("linux") && !qbs.targetOS.contains("android")

    minimumQbsVersion: "1.9.0"

    references: [
        "src/src.qbs"
    ]
}

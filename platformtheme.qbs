import qbs 1.0

Project {
    name: "QPA Theme"

    readonly property string version: "1.0.0"

    property bool useStaticAnalyzer: false

    condition: qbs.targetOS.contains("linux") && !qbs.targetOS.contains("android")

    minimumQbsVersion: "1.8.0"

    references: [
        "src/src.qbs"
    ]
}

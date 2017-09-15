import qbs 1.0

Project {
    name: "QPA Theme"

    readonly property string version: "1.0.0"

    condition: qbs.targetOS.contains("linux")

    minimumQbsVersion: "1.8.0"

    references: [
        "src/src.qbs"
    ]
}

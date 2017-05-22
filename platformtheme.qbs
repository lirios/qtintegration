import qbs 1.0

Project {
    name: "QPA Theme"

    readonly property string version: "1.0.0"

    condition: qbs.targetOS.contains("linux")

    minimumQbsVersion: "1.6"

    qbsSearchPaths: "qbs-shared"

    references: [
        "src/src.qbs"
    ]
}

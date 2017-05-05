import qbs 1.0

Project {
    name: "QPA Theme"

    readonly property string version: "1.0.0"

    minimumQbsVersion: "1.6"

    qbsSearchPaths: base.concat(["qbs"])

    references: [
        "src/src.qbs"
    ]

    Product {
        name: "qmake project files for liri-platform-theme"
        files: [
            "**/*.pr[io]"
        ]
    }
}

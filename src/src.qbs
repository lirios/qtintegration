import qbs 1.0
import qbs.Probes

DynamicLibrary {
    name: "liritheme"
    targetName: "liritheme"

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["core", "gui", "gui-private", "widgets", "quickcontrols2"] }
    Depends { name: "qbsbuildconfig" }
    Depends { name: "libQtGSettings" }

    cpp.defines: [
        "QT_NO_CAST_FROM_ASCII",
        "QT_NO_CAST_TO_ASCII"
    ]

    files: ["*.cpp", "*.h"]

    Group {
        qbs.install: true
        qbs.installDir: (qbsbuildconfig.useQtPaths ? Qt.core.pluginPath : "lib/plugins") + "/platformthemes"
        fileTagsFilter: product.type
    }
}

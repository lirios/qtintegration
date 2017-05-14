import qbs 1.0
import qbs.Probes

LiriDynamicLibrary {
    name: "liritheme"
    targetName: "liritheme"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["core", "gui", "gui-private", "widgets", "quickcontrols2"] }
    Depends { name: "Qt5GSettings" }

    files: ["*.cpp", "*.h"]

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.pluginsDir + "/platformthemes"
        fileTagsFilter: product.type
    }
}

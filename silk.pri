isEmpty(SILK_PRI) { SILK_PRI = 1
    SILK_VERSION = 0.0.1

    include(./silkplatform.pri)

    defineReplace(qtLibraryName) {
        unset(LIBRARY_NAME)
        LIBRARY_NAME = $$1
        CONFIG(debug, debug|release) {
            !debug_and_release|build_pass {
                silk_platform_mac: RET = $$member(LIBRARY_NAME, 0)_debug
                else: silk_platform_windows : RET = $$member(LIBRARY_NAME, 0)d
            }
        }
        isEmpty(RET): RET = $$LIBRARY_NAME
        return($$RET)
    }

    defineReplace(cleanPath) {
        silk_platform_windows: 1 ~= s|\\\\|/|g
        contains(1, ^/.*): pfx = /
        else: pfx =
        segs = $$split(1, /)
        out =
        for(seg, segs) {
            equals(seg, ..): out = $$member(out, 0, -2)
            else:!equals(seg, .): out += $$seg
        }
        return($$join(out, /, $$pfx))
    }

    SILK_SOURCE_TREE = $$PWD
    INCLUDEPATH += $$SILK_SOURCE_TREE/src

    isEmpty(SILK_BUILD_TREE) {
        sub_dir = $$_PRO_FILE_PWD_
        sub_dir ~= s,^$$re_escape($$PWD),,
        SILK_BUILD_TREE = $$cleanPath($$OUT_PWD)
        SILK_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
    }

    silk_platform_mac {
        SILK_APP_TARGET = Silk
        SILK_TARGET_PATH = $${SILK_APP_TARGET}.app/Contents
        SILK_APP_PATH = MacOS
        SILK_LIBRARY_PATH = PlugIns
        SILK_PLUGIN_PATH = $$SILK_LIBRARY_PATH/$$SILK_APP_TARGET
        SILK_IMPORTS_PATH = $$SILK_LIBRARY_PATH/Imports
        SILK_DATA_PATH = Resources
        SILK_TRANSLATIONS_PATH = $$SILK_DATA_PATH/translations
        QMAKE_CXXFLAGS *= -mmacosx-version-min=10.5
        QMAKE_LFLAGS *= -mmacosx-version-min=10.5
        copydata = 1
    }

    silk_platform_linux {
        SILK_APP_TARGET = silk
        SILK_TARGET_PATH =
        SILK_APP_PATH = bin
        SILK_LIBRARY_PATH = lib
        SILK_PLUGIN_PATH = $$SILK_LIBRARY_PATH/$$SILK_APP_TARGET/plugins
        SILK_IMPORTS_PATH = $$SILK_LIBRARY_PATH/$$SILK_APP_TARGET/imports
        SILK_DATA_PATH = share/$$SILK_APP_TARGET
        SILK_TRANSLATIONS_PATH = $$SILK_DATA_PATH/translations
        !isEqual(SILK_SOURCE_TREE, $$SILK_BUILD_TREE):copydata = 1
    }

    silk_platform_windows {
        SILK_APP_TARGET = silk
        SILK_TARGET_PATH =
        SILK_APP_PATH = bin
        SILK_LIBRARY_PATH = bin
        SILK_PLUGIN_PATH = lib/$$SILK_APP_TARGET/plugins
        SILK_IMPORTS_PATH = lib/$$SILK_APP_TARGET/imports
        SILK_DATA_PATH = share/$$SILK_APP_TARGET
        SILK_TRANSLATIONS_PATH = $$SILK_DATA_PATH/translations
        !isEqual(SILK_SOURCE_TREE, $$SILK_BUILD_TREE):copydata = 1
    }

    DEFINES += SILK_VERSION=\\\"$$SILK_VERSION\\\"
    DEFINES += SILK_APP_PATH=\\\"$$SILK_APP_PATH\\\"
    DEFINES += SILK_LIBRARY_PATH=\\\"$$SILK_LIBRARY_PATH\\\"
    DEFINES += SILK_PLUGIN_PATH=\\\"$$SILK_PLUGIN_PATH\\\"
    DEFINES += SILK_IMPORTS_PATH=\\\"$$SILK_IMPORTS_PATH\\\"
    DEFINES += SILK_DATA_PATH=\\\"$$SILK_DATA_PATH\\\"
    DEFINES += SILK_TRANSLATIONS_PATH=\\\"$$SILK_TRANSLATIONS_PATH\\\"

    CONFIG(debug, debug|release):OBJECTS_DIR = $${OUT_PWD}/.obj/debug-shared
    CONFIG(release, debug|release):OBJECTS_DIR = $${OUT_PWD}/.obj/release-shared

    CONFIG(debug, debug|release):MOC_DIR = $${OUT_PWD}/.moc/debug-shared
    CONFIG(release, debug|release):MOC_DIR = $${OUT_PWD}/.moc/release-shared

    RCC_DIR = $${OUT_PWD}/.rcc
    UI_DIR = $${OUT_PWD}/.uic

    defineTest(versionCheck) {
        major = $$1
        minor = $$2
        patch = $$3

        isEqual(QT_MAJOR_VERSION, $$major) {
            isEqual(QT_MINOR_VERSION, $$minor) {
                isEqual(QT_PATCH_VERSION, $$patch) {
                    return(true)
                }
                greaterThan(QT_PATCH_VERSION, $$patch) {
                    return(true)
                }
            }
            greaterThan(QT_MINOR_VERSION, $$minor) {
                return(true)
            }
        }
        greaterThan(QT_MAJOR_VERSION, $$major) {
            return(true)
        }
        return(false)
    }
}

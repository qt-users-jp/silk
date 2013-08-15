isEmpty(SILKLIBRARY_PRI) { SILKLIBRARY_PRI = 1

    TEMPLATE = lib
    CONFIG += console

    include(./silk.pri)

    contains(QT_CONFIG, reduce_exports) {

        contains(CONFIG, qt): CONFIG += hide_symbols
    }

    TARGET = $$qtLibraryTarget($$TARGET)

    DESTDIR = $$SILK_BUILD_TREE/$$SILK_TARGET_PATH/$$SILK_LIBRARY_PATH
    silk_platform_windows {
        DLLDESTDIR = $$SILK_BUILD_TREE/$$SILK_TARGET_PATH/$$SILK_LIBRARY_PATH
    }
    silk_platform_mac {
        QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/PlugIns/
    }

    CONFIG(shared, static|shared) {
        silk_platform_mac | silk_platform_linux {
            target.path = $$PREFIX/$$SILK_TARGET_PATH/$$SILK_LIBRARY_PATH
            INSTALLS += target
        }
    }
}

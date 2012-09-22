isEmpty(SILKPLUGIN_PRI) { SILKPLUGIN_PRI = 1

    include(./silk.pri)
    include(./src/libs/libs.pri)

    contains(QT_CONFIG, reduce_exports) {
        contains(CONFIG, qt): CONFIG += hide_symbols
    }

    TEMPLATE = lib
    CONFIG += plugin

    TARGET = $$qtLibraryTarget($$TARGET)

    DESTDIR = $$SILK_BUILD_TREE/$$SILK_TARGET_PATH/$$SILK_PLUGIN_PATH/

    isEmpty(target.path) {
        target.path = $$PREFIX/$$SILK_TARGET_PATH/$$SILK_PLUGIN_PATH/
    }
    INSTALLS += target

    QMAKE_RPATHDIR += \$\$ORIGIN/../../../$$SILK_LIBRARY_PATH
    include(./silkrpath.pri)
}

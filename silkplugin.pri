isEmpty(SILKPLUGIN_PRI) { SILKPLUGIN_PRI = 1

    TEMPLATE = lib
    CONFIG += plugin
    QT -= gui

    include(./silk.pri)
    include(./src/lib/lib.pri)

    contains(QT_CONFIG, reduce_exports): CONFIG += hide_symbols

    TARGET = $$qtLibraryTarget($$TARGET)

    DESTDIR = $$SILK_BUILD_TREE/$$SILK_TARGET_PATH/$$SILK_PLUGIN_PATH/$$SILK_PLUGIN_TYPE/

    isEmpty(target.path) {
        target.path = $$PREFIX/$$SILK_TARGET_PATH/$$SILK_PLUGIN_PATH/$$SILK_PLUGIN_TYPE/
    }
    INSTALLS += target

    QMAKE_RPATHDIR += \$\$ORIGIN/../../../../$$SILK_LIBRARY_PATH
    include(./silkrpath.pri)
}

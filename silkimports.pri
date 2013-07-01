isEmpty(SILKIMPORTS_PRI) { SILKIMPORTS_PRI = 1

    TEMPLATE = lib
    CONFIG += plugin
    QT -= gui
    QT += qml

    include(./silk.pri)
    include(./src/lib/lib.pri)

    contains(QT_CONFIG, reduce_exports): CONFIG += hide_symbols

    TARGET = $$qtLibraryTarget($$TARGET)

    DESTDIR = $$SILK_BUILD_TREE/$$SILK_TARGET_PATH/$$SILK_IMPORTS_PATH/$$TARGETPATH

    CONFIG(shared, static|shared) {
        isEmpty(target.path) {
            target.path = $$PREFIX/$$SILK_TARGET_PATH/$$SILK_IMPORTS_PATH/$$TARGETPATH
        }
        INSTALLS += target

        QMAKE_RPATHDIR += \$\$ORIGIN/../../
        include(./silkrpath.pri)
    }
}

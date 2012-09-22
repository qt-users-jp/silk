isEmpty(SILKTEST_PRI) { SILKTEST_PRI = 1

    TEMPLATE = app
    QT += testlib
    CONFIG += testcase

    include(./silk.pri)

    DESTDIR = $$SILK_BUILD_TREE/$$SILK_TARGET_PATH/$$SILK_APP_PATH
    QMAKE_RPATHDIR += \$\$ORIGIN/../$$SILK_LIBRARY_PATH

    include(./silkrpath.pri)
    include(./src/lib/lib.pri)
}

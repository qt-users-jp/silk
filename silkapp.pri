isEmpty(SILKAPP_PRI) { SILKAPP_PRI = 1

    TEMPLATE = app
    include(./silk.pri)

    DESTDIR = $$SILK_BUILD_TREE/$$SILK_TARGET_PATH/$$SILK_APP_PATH

    QMAKE_RPATHDIR += \$\$ORIGIN/../$$SILK_LIBRARY_PATH
    include(./silkrpath.pri)
    include(./src/lib/lib.pri)

    silk_platform_mac | silk_platform_linux {
        target.path = $$PREFIX/$$SILK_APP_PATH
        INSTALLS += target
    }
}

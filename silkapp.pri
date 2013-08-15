isEmpty(SILKAPP_PRI) { SILKAPP_PRI = 1

    TEMPLATE = app
    QT -= gui
    QT += network

    include(./silk.pri)

    silk_platform_mac {
        TARGET = Silk
        DESTDIR = $$SILK_BUILD_TREE
    } else {
        DESTDIR = $$SILK_BUILD_TREE/$$SILK_TARGET_PATH/$$SILK_APP_PATH
    }

    CONFIG(static, static|shared) {
        QT += qml
        LIBS += -L$$[QT_INSTALL_QML]/QtQml/Models.2/ -lmodelsplugin

        LIBS += -L$$SILK_BUILD_TREE/$$SILK_TARGET_PATH/$$SILK_PLUGIN_PATH/mimehandler -ldeflate -lqml

        LIBS += -L$$SILK_BUILD_TREE/$$SILK_TARGET_PATH/$$SILK_PLUGIN_PATH/protocolhandler -lhttp

        LIBS += -L$$SILK_BUILD_TREE/$$SILK_TARGET_PATH/$$SILK_IMPORTS_PATH
        LIBS += -lBootstrap -lCSS -lCache -lHTML -lJSON -lOAuth -lProcess -lRSS -lSMTP -lUtils -lXML
    } else {
        QMAKE_RPATHDIR += \$\$ORIGIN/../$$SILK_LIBRARY_PATH
        include(./silkrpath.pri)
    }
    include(./src/lib/lib.pri)

    silk_platform_mac | silk_platform_linux {
        target.path = $$PREFIX/$$SILK_APP_PATH
        INSTALLS += target
    }
}

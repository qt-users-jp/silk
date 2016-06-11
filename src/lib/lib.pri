QT += qml

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

include(../../silk.pri)

LIBS *= -L$$SILK_BUILD_TREE/$$SILK_TARGET_PATH/$$SILK_LIBRARY_PATH

LIBS *= -l$$qtLibraryName(silk)

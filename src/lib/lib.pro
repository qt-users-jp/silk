TEMPLATE = lib
TARGET = silk

include(../../silklib.pri)

DEFINES += SILK_LIBRARY

HEADERS += \
    silkglobal.h \
    silkimportsinterface.h \
    silkabstracthttpobject.h

SOURCES += \
    silkabstracthttpobject.cpp

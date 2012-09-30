TEMPLATE = lib
TARGET = silk

QT -= gui

include(../../silklib.pri)
include(../../qthttpserver/qthttpserver.pri)

DEFINES += SILK_LIBRARY

HEADERS += \
    silkglobal.h \
    silkconfig.h \
    silkimportsinterface.h \
    silkabstracthttpobject.h \
    silkmimehandlerinterface.h \
    silkabstractmimehandler.h \
    silkabstractobject.h \
    silkserver.h

SOURCES += \
    silkconfig.cpp \
    silkabstracthttpobject.cpp \
    silkabstractmimehandler.cpp \
    silkabstractobject.cpp \
    silkserver.cpp

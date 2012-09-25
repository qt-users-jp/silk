TEMPLATE = lib
TARGET = silk

include(../../silklib.pri)
include(../../qthttpserver/qthttpserver.pri)

DEFINES += SILK_LIBRARY

HEADERS += \
    silkglobal.h \
    silkconfig.h \
    silkimportsinterface.h \
    silkabstracthttpobject.h \
    silkmimehandlerinterface.h \
    silkabstractmimehandler.h

SOURCES += \
    silkconfig.cpp \
    silkabstracthttpobject.cpp \
    silkabstractmimehandler.cpp

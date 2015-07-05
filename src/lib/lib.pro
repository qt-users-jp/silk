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
    silkprotocolhandlerinterface.h \
    silkabstractprotocolhandler.h \
    silkabstractobject.h \
    silkserver.h \
    silkserver_logging.h

SOURCES += \
    silkconfig.cpp \
    silkabstracthttpobject.cpp \
    silkabstractmimehandler.cpp \
    silkabstractprotocolhandler.cpp \
    silkabstractobject.cpp \
    silkserver.cpp \
    silkserver_logging.cpp

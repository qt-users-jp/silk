TARGET = silk

include(../../silkapp.pri)

QT -= gui
QT += qml

include(../3rdparty/qtservice/src/qtservice.pri)
include(../../qthttpserver/qthttpserver.pri)
include(../../etc/etc.pri)
include(../../contents/contents.pri)

SOURCES += main.cpp \
    silk.cpp \
    silkservice.cpp \
    silkconfig.cpp \
    silkhttpobject.cpp

HEADERS += \
    silk.h \
    silkservice.h \
    silkconfig.h \
    silkhttpobject.h

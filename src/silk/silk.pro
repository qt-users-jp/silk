TARGET = silk

include(../../silkapp.pri)

QT -= gui

include(../3rdparty/qtservice/src/qtservice.pri)
include(../../etc/etc.pri)
include(../../contents/contents.pri)

SOURCES += main.cpp \
    silk.cpp \
    silkservice.cpp

HEADERS += \
    silk.h \
    silkservice.h

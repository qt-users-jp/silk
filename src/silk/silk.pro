TARGET = silk

include(../../silkapp.pri)

QT -= gui
QT += network

include(../../etc/etc.pri)
include(../../contents/contents.pri)

SOURCES += main.cpp \
    silk.cpp

HEADERS += \
    silk.h

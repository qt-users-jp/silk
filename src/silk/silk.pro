TARGET = silk

include(../../silkapp.pri)

QT -= gui
QT += network qml

include(../../etc/etc.pri)
include(../../contents/contents.pri)

SOURCES += main.cpp \
    silkserver.cpp \
    silkapplication.cpp

HEADERS += \
    silkserver.h \
    silkapplication.h

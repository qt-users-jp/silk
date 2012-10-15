SILK_PLUGIN_TYPE = mimehandler
include(../../../../silkplugin.pri)

QT += network qml

HEADERS += \
    qmlplugin.h \
    qmlhandler.h \
    httpobject.h \
    silk.h \
    websocketobject.h \
    text.h

SOURCES += \
    qmlhandler.cpp \
    httpobject.cpp \
    silk.cpp \
    websocketobject.cpp \
    text.cpp

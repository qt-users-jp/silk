SILK_PLUGIN_TYPE = mimehandler
include(../../../../silkplugin.pri)

QT += network qml

HEADERS += \
    qmlplugin.h \
    qmlhandler.h \
    httpobject.h \
    silk.h

SOURCES += \
    qmlhandler.cpp \
    httpobject.cpp \
    silk.cpp

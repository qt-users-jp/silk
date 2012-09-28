SILK_PLUGIN_TYPE = mimehandler
include(../../../../silkplugin.pri)

QT += network

HEADERS += \
    proxyplugin.h \
    proxyhandler.h

SOURCES += \
    proxyhandler.cpp

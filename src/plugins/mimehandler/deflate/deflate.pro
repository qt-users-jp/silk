SILK_PLUGIN_TYPE = mimehandler
include(../../../../silkplugin.pri)

QT = core httpserver

HEADERS += \
    deflateplugin.h \
    deflatehandler.h

SOURCES += \
    deflatehandler.cpp

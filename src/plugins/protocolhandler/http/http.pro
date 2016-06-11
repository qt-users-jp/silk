SILK_PLUGIN_TYPE = protocolhandler
include(../../../../silkplugin.pri)

QT = httpserver

HEADERS += \
    httpplugin.h \
    httphandler.h

SOURCES += \
    httphandler.cpp

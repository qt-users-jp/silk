SILK_PLUGIN_TYPE = protocolhandler
include(../../../../silkplugin.pri)

QT += network

HEADERS += \
    httpplugin.h \
    httphandler.h

SOURCES += \
    httphandler.cpp

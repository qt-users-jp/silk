SILK_PLUGIN_TYPE = protocolhandler
include(../../../../../../silkplugin.pri)

QT += network sql

HEADERS += \
    sssoplugin.h \
    sssohandler.h

SOURCES += \
    sssohandler.cpp

TEMPLATE = app
TARGET = silk
DESTDIR = ../bin
QT -= gui
QT += qml

include(../3rdparty/qtservice/src/qtservice.pri)
include(../qthttpserver/qthttpserver.pri)
include(./imports/imports.pri)

SOURCES += main.cpp \
    silk.cpp \
    qhttpservice.cpp

HEADERS += \
    silk.h \
    qhttpservice.h

RESOURCES += \
    src.qrc

contents.source = contents
contents.target = .

DEPLOYMENTFOLDERS = contents

include(./deployment.pri)
qtcAddDeployment()

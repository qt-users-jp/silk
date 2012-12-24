include(../../../../silkimports.pri)

QT += sql qml
QT -= gui

HEADERS += \
    databaseplugin.h \
    database.h \
    tablemodel.h \
    selectsqlmodel.h

SOURCES += \
    database.cpp \
    tablemodel.cpp \
    selectsqlmodel.cpp


include(../../../../../silkimports.pri)

QT += sql

HEADERS += \
    carpetplugin.h \
    backup.h \
    backupjob.h \
    abstractcarpetjob.h \
    addexcludepatternjob.h \
    removeexcludepatternjob.h \
    updatestatisticsjob.h \
    abstractcarpetjobprivate_p.h \
    filelistmodel.h \
    carpetdb.h \
    frequentlychangedmodel.h \
    recentlychangedmodel.h \
    sizeconsumedmodel.h \
    pathmodel.h \
    carpetabstractmodel.h \
    carpetfileinfo.h \
    entrymodel.h \
    historymodel.h

SOURCES += \
    backup.cpp \
    backupjob.cpp \
    abstractcarpetjob.cpp \
    addexcludepatternjob.cpp \
    removeexcludepatternjob.cpp \
    updatestatisticsjob.cpp \
    abstractcarpetjobprivate_p.cpp \
    filelistmodel.cpp \
    carpetdb.cpp \
    frequentlychangedmodel.cpp \
    recentlychangedmodel.cpp \
    sizeconsumedmodel.cpp \
    pathmodel.cpp \
    carpetabstractmodel.cpp \
    entrymodel.cpp \
    historymodel.cpp

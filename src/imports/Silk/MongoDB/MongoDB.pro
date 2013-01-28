include(../../../../silkimports.pri)
include(./3rdparty/mongo-c-driver.pri)

HEADERS += \
    mongodatabase.h \
    mongodbplugin.h \
    mongocollection.h \
    mongoquery.h

SOURCES += \
    mongodatabase.cpp \
    mongocollection.cpp \
    mongoquery.cpp

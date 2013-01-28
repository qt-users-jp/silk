INCLUDEPATH += $$PWD/mongo-c-driver/src
DEPENDPATH += $$PWD/mongo-c-driver/src
DEFINES += MONGO_HAVE_STDINT

HEADERS += $$PWD/mongo-c-driver/src/bcon.h \
           $$PWD/mongo-c-driver/src/bson.h \
           $$PWD/mongo-c-driver/src/encoding.h \
           $$PWD/mongo-c-driver/src/env.h \
           $$PWD/mongo-c-driver/src/gridfs.h \
           $$PWD/mongo-c-driver/src/md5.h \
           $$PWD/mongo-c-driver/src/mongo.h

SOURCES += $$PWD/mongo-c-driver/src/bcon.c \
           $$PWD/mongo-c-driver/src/bson.c \
           $$PWD/mongo-c-driver/src/encoding.c \
           $$PWD/mongo-c-driver/src/env.c \
           $$PWD/mongo-c-driver/src/gridfs.c \
           $$PWD/mongo-c-driver/src/md5.c \
           $$PWD/mongo-c-driver/src/mongo.c \
           $$PWD/mongo-c-driver/src/numbers.c

TARGET = blog
include(../../silkapp.pri)

SOURCES += main.cpp

RESOURCES += \
    blog.qrc

OTHER_FILES += \
    blogrc \
    frontend/index.qml \
    frontend/Theme.qml \
    frontend/UserInput.qml \
    frontend/Twitter.qml \
    frontend/AbstractSlugModel.qml \
    frontend/UserModel.qml \
    frontend/TagModel.qml \
    frontend/ArticleModel.qml \
    frontend/edit.js \
    frontend/plugins/include.qml \
    frontend/plugins/api/Plugin.qml \
    frontend/plugins/highlight/qml.js \
    frontend/plugins/highlight/json.js


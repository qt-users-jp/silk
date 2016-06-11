TEMPLATE = subdirs
CONFIG += ordered

qtHaveModule(qml): qtHaveModule(httpserver) {
    SUBDIRS = src contents demos
} else {
    error(silk requires QtQml and QtHttpserver installed.)
}

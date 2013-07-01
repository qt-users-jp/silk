import QtQml 2.0

Nav {
    id: root

    Component.onCompleted: {
        var c = root._class.split(' ')
        c.unshift('nav-list')
        root._class = c.join(' ')
    }
}

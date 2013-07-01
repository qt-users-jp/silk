import QtQml 2.0

Alert {
    id: root

    Component.onCompleted: {
        var c = root._class.split(' ')
        c.unshift('alert-block')
        root._class = c.join(' ')
    }
}

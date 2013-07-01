import QtQml 2.0
import Silk.HTML 5.0 as HTML

HTML.Li {
    id: root

    Component.onCompleted: {
        var c = root._class.split(' ')
        c.push('nav-header')
        root._class = c.join(' ')
    }
}

import QtQml 2.0
import Silk.HTML 5.0 as HTML

HTML.Ul {
    id: root

    Component.onCompleted: {
        var c = root._class.split(' ')
        c.push('nav')
        c.push('nav-tabs')
        root._class = c.join(' ')
    }
}

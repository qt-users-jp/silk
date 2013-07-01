import QtQml 2.0
import Silk.HTML 5.0 as HTML

HTML.Div {
    id: root

    Component.onCompleted: {
        var c = root._class.split(' ')
        c.push('btn-group')
        root._class = c.join(' ')
    }
}

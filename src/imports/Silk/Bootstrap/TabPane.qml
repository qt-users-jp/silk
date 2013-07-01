import QtQml 2.0
import Silk.HTML 5.0 as HTML

HTML.Ul {
    id: root

    property bool __active: false

    Component.onCompleted: {
        var c = root._class.split(' ')
        c.push('tab-pane')
        if (root.__active)
            c.push('active')
        root._class = c.join(' ')
    }
}

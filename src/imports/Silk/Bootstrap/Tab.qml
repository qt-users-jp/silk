import QtQml 2.0
import Silk.HTML 5.0 as HTML

HTML.Li {
    id: root

    property bool __active: false
    property alias __pane: button.href

    HTML.A {
        id: button
        property string data_toggle: 'tab'
    }

    Component.onCompleted: {
        var c = root._class.split(' ')
        if (root.__active)
            c.push('active')
        root._class = c.join(' ')
        button.text = root.text
        root.text = ''
    }
}

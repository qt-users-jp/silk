import QtQml 2.0
import Silk.HTML 5.0 as HTML

HTML.Div {
    id: root

    default property alias contents: inner.contents

    Component.onCompleted: {
        var c = root._class.split(' ')
        c.push('navbar')
        root._class = c.join(' ')
    }

    HTML.Div {
        id: inner
        _class: 'navbar-inner'
    }
}

import QtQml 2.0
import Silk.HTML 5.0 as HTML

HTML.Div {
    id: root

    property string __purpose: ''
    property string __size: ''

    Component.onCompleted: {
        var c = root._class.split(' ')
        c.push('btn')
        if (root.__size.length > 0)
            c.push('btn-%1'.arg(root.__size))
        if (root.__purpose.length > 0)
            c.push('btn-%1'.arg(root.__purpose))
        root._class = c.join(' ')
    }
}

import QtQml 2.0
import Silk.HTML 5.0 as HTML

ButtonGroup {
    id: root
    default property alias contents: menu.contents

    HTML.A {
        id: button
        _class: 'btn dropdown-toggle'
        property string data_toggle: 'dropdown'
        href: '#'

        HTML.Span { _class: 'caret' }
    }

    HTML.Ul {
        id: menu
        _class: 'dropdown-menu'
    }

    Component.onCompleted: {
        button.text = root.text
        root.text = ''
    }
}

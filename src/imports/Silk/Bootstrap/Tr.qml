import Silk.HTML 5.0 as HTML

HTML.Tr {
    _class: ''
            + (__success ? ' success' : '')
            + (__error ? ' error' : '')
            + (__info ? ' info' : '')

    property bool __success: false
    property bool __error: false
    property bool __info: false
}

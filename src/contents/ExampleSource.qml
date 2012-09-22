import QtQuick.HTTP 1.1
import QtQuick.HTML 5.0

Http {
    status: 200
    responseHeader: {"Content-Type": "text/html; charset=utf-8;"}

    DocType {}

    Html {
        Head { Title { id: title; text: "HTML written in QML" } }
        Body {
            Article {
                Header {
                    H1 { text: title.text }
                }
                Section {
                    P {
                        text: 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam sodales urna non odio egestas tempor. Nunc vel vehicula ante. Etiam bibendum iaculis libero, eget molestie nisl pharetra in. In semper consequat est, eu porta velit mollis nec. Curabitur posuere enim eget turpis feugiat tempor. Etiam ullamcorper lorem dapibus velit suscipit ultrices. Proin in est sed erat facilisis pharetra.'
                    }
                }
                Footer {
                    P { text: 'Code less, Create more!' }
                }
            }

        }
    }
}

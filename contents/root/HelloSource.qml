import Silk.HTTP 1.1
import Silk.HTML 5.0

Http {
    status: 200
    responseHeader: {"Content-Type": "text/html; charset=utf-8;"}

    DocType {}

    Html {
        Head { Title { id: title; text: "Hello silk world" } }
        Body {
            Article {
                Header {
                    H1 { text: title.text }
                }
                Section {
                    P { text: "Beautiful, isn't it?" }
                }
                Footer {
                    P { text: "This HTML is generated from a QML file" }
                }
            }
        }
    }
}

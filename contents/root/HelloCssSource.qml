/* hellocss.qml */
import Silk.HTTP 1.1
import Silk.CSS 3.0

Http {
    status: 200
    responseHeader: {"Content-Type": "text/css; charset=utf-8;"}

    Rule {
        selector: 'h1'
        color: '#006225'
    }

    Rule {
        selector: 'section'
        background: '#80C342'

        Rule {
            selector: 'p'
            color: 'white'
            font_weight: 'bold'
        }
    }

    Rule {
        selector: 'footer p'
        text_decoration: 'underline'
    }
}

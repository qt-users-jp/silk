/* hellocss.qml */
import Silk.CSS 3.0

Css {
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

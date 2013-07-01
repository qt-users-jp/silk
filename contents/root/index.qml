/* Copyright (c) 2012 - 2013 Silk Project.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Silk nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL SILK BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

import Silk.HTML 5.0
import Silk.Bootstrap 2.3 as Bootstrap
import 'components'

SilkPageTemplate {
    Head {
        Title { text: 'silk - simple and flexible web framework' }
        Link { href: '/css/archtecture.qml'; rel: 'stylesheet'; media: 'screen' }
    }

    Bootstrap.HeroUnit {
        heading: 'Simple and flexible web framework'
        tagline: 'Write everything in QML and JavaScript'
        Table {
            property string style: "width: 100%; border-collapse: separate; border-spacing: 5px; table-layout: fixed;"
            Tr {
                ArchitectureBlock {
                    rowspan: "2"; __class: "arch_html arch_op50"
                    Text { text: "HTML" }
                }
                ArchitectureBlock {
                    colspan: "2"; __class: "arch_html arch_op50"
                    Text { text: "HTML" }
                }
                ArchitectureBlock {
                    colspan: "2"; __class: "arch_js arch_op50"
                    Text { text: "XML / JSON" }
                }
                ArchitectureBlock { text: "CSS"; __class: "arch_css arch_op50" }
                ArchitectureBlock { text: "Java<br />Script"; rowspan: "2"; __class: "arch_js" }
            }
            Tr {
                ArchitectureBlock {
                    text: "QML with JavaScript"
                    colspan: "5"
                    __class: "arch_qml"
                }
            }
            Tr {
                ArchitectureBlock {
                    text: "silk web server"
                    colspan: "7"
                    __class: "arch_qt arch_op50"
                }
            }
        }
    }

    Bootstrap.RowFluid {
        Bootstrap.Span4 {
            H2 { A { href: '/scaffolding.qml#html'; text: 'HTML' } }
            Pre {
                text: "import Silk.HTML 5.0

Html {
    DocType {}
    Head { ... }
    Body {
        H1 { text: 'title' }
        P { text: '...' }
    }
}"
            }
        }

        Bootstrap.Span4 {
            H2 { A { href: '/scaffolding.qml#xml'; text: 'XML' } }
            Pre {
                text: "import Silk.XML 1.0

Tag {
    tagName: 'tag'
    text: 'text'
}"
            }
        }

        Bootstrap.Span4 {
            H2 { A { href: '/scaffolding.qml#json'; text: 'JSON' } }
            Pre {
                text: "import Silk.JSON 1.0

Json {
    object: {
        'key': 'width'
        , 'value': 100
    }
}"
            }
        }
    }

    Bootstrap.RowFluid {
        Bootstrap.Span4 {
            H2 { A { href: '/scaffolding.qml#css'; text: 'CSS' } }
            Pre {
                text: "import Silk.CSS 3.0

Css {
    Rule {
        selector: 'body'
        background_color: '0'
    }
    ...
}"
            }
        }

        Bootstrap.Span4 {
            H2 { A { href: '/scaffolding.qml#text'; text: 'Text' } }
            Pre {
                text: "import Silk.Text 1.0

Text {
    text: 'Hello World!'
}"
            }
        }

        Bootstrap.Span4 {
            H2 { A { href: '/examples.qml#communication'; text: 'WebSocket' } }
            Pre {
                text: "import Silk.WebSocket 1.0

WebSocket {
    onReady: accept()
    onMessage: send(message.data);
}"
            }
        }
    }
}

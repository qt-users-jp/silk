/* Copyright (c) 2012 Silk Project.
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
import "../"
import "../components/"

SilkPageTemplate {
    subtitle: "Hello QML"

    Article {
//        Header {
//            H2 { text: "Hello QML" }
//        }

        Section {
            H2 { text: "a QML is a plain text file" }
            P {
                text: "Use your favorite editor to write QML files."
            }
        }

        Section {
            H2 { text: "Simple QML example" }
            PlainFile {
                __class: 'qml'
                text: '/*
 *   MyElement.qml // (1)
 */
import Silk.HTML 5.0 // (2)

Div { // (3)
    A { // (4)
        href: "http://silk.qtquick.me/" // (5)
        text: "silk" // (6)
    }
}
'
            }
            Dl {
                Dt { text: '(1) comment' }
                Dd {
                    Text { text: 'Any text between /* and */ and after // will be ignored.' }
                    PlainFile {
                        __class: 'qml'
                        text: '/* This is a comment */'
                    }
                    PlainFile {
                        __class: 'qml'
                        text: '// This is a comment'
                    }
                    PlainFile {
                        __class: 'qml'
                        text: '/*
    This is a comment
*/'
                    }
                }

                Dt { text: '(2) import modules' }
                Dd {
                    Text { text: 'This line imports a <i style="color: red;">module</i> called <code>Silk.HTML</code> version <code>5.0</code> which provides html5 tag elements.' }
                    PlainFile {
                        __class: 'qml'
                        text: '// import Silk.HTTP module version 1.1
import Silk.HTTP 1.1'
                    }
                    PlainFile {
                        __class: 'qml'
                        text: '// import Silk.CSS module version 3.0
import Silk.CSS 3.0
'
                    }
                }

                Dt { text: '(3) (4) use elements' }
                Dd {
                    Text {text: 'Elements can be used in the <i>Element { ... }</i> style' }
                    PlainFile {
                        __class: 'qml'
                        text: '// use Html element
Html {}'
                    }
                    PlainFile {
                        __class: 'qml'
                        text: '// child elements in elements
Html {
    Head { ... }
    Body {
        P { ... }
    }
}'
                    }
                    PlainFile {
                        __class: 'qml'
                        text: '// a QML file must have exactly one root element in it
Div { ... }
<del>Div { ... }</del> // must not have multiple root elements'
                    }
                    PlainFile {
                        __class: 'qml'
                        text: '// name of elements starts with one ore more Capital letters
Div {
    A { ... }
    a { ... } // this is not valid
}'
                    }
                }
                Dt { text: '(5) (6) set properties' }
                Dd {
                    Text { text: 'Each elements have their own properties and can be set like <i>name: value</i>.' }
                    PlainFile {
                        __class: 'qml'
                        text: '// set "status" property to 200 in Http element
Http {
    // the property type is defined as int
    status: 200
}'
                    }
                    PlainFile {
                        __class: 'qml'
                        text: '// set "lang" property in Html element
Html {
    // the property type is defined as string
    lang: "ja"
}
// will be &lt;html lang="ja" /&gt;'
                    }
                    PlainFile {
                        __class: 'qml'
                        text: '// "text" is a special property for innerText
P {
    text: "hello"
}
// will be &lt;p&gt;hello&lt;/p&gt; not &lt;p text="hello" /&gt;'
                    }
                    PlainFile {
                        __class: 'qml'
                        text: '// "id" is a special property that can be referenced by others
Html {
    Head {
        Title {
            id: title // id must be unique in a QML file
            text: "Hello"
        }
    }
    Body {
        // the text value will be the value of text property of title
        H1 { text: title.text }
    }
}'
                    }
                }
            }
        }

        Footer {
            Nav {
                A { href: "/config.qml"; text: "Configuration" }
                Text { text: " &lt;&lt; Previous / Next &gt;&gt; " }
                A { href: "/examples/"; text: "Examples" }
            }
        }
    }
}

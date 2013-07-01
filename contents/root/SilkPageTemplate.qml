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
import Silk.CSS 3.0
import Silk.Utils 1.0
import Silk.Bootstrap 2.3 as Bootstrap
import QtQml 2.1
import QtQml.Models 2.1

Html {
    id: root

    default property alias contents: main.contents

    property var data_spy: ''
    property var data_target: ''
    property var data_offset: ''

//    DocType {}

    Head {
        id: head
        Meta { charset: "utf-8" }
//        Meta { name: "description"; content: "" }
        Meta { name: "viewport"; content: "width=device-width, initial-scale=1.0" }
//        Link { href: 'http://twitter.github.io/bootstrap/assets/css/bootstrap.css'; rel: 'stylesheet'; media: 'screen' }
//        Link { href: 'http://twitter.github.io/bootstrap/assets/css/bootstrap-responsive.css'; rel: 'stylesheet'; media: 'screen' }
        Link { href: '/3rdparty/css/bootstrap.css'; rel: 'stylesheet'; media: 'screen' }
        Link { href: '/3rdparty/css/bootstrap-responsive.css'; rel: 'stylesheet'; media: 'screen' }
        Link { href: '/3rdparty/css/docs.css'; rel: 'stylesheet'; media: 'screen' }
        Link { href: '/css/'; rel: 'stylesheet'; media: 'screen' }
//        Script { type: 'text/javascript'; src: "/js/gin.js" }
//        Script { type: 'text/javascript'; src: "/js/highlighter.js" }
    }

    Body {
        id: body
        property string data_spy: root.data_spy
        property string data_target: root.data_target
        property string data_offset: root.data_offset
//        onload: "highlight()"

        Bootstrap.Navbar {
            _class: 'navbar-fixed-top'
            Bootstrap.Container {
                Button {
                    type: 'button'
                    _class: 'btn btn-navbar'
                    property string data_toggle: 'collapse'
                    property string data_target: '.nav-collapse'

                    Span { _class: 'icon-bar'; text: '' }
                    Span { _class: 'icon-bar'; text: '' }
                    Span { _class: 'icon-bar'; text: '' }
                }

                A {
                    _class: 'brand'
                    href: '/'
                    text: 'Silk'
                }

                Div {
                    _class: 'nav-collapse collapse'
                    Bootstrap.Nav {
                        Repeater {
                            model: ListModel {
                                ListElement {
                                    path: '/'
                                    title: 'Home'
                                }
                                ListElement {
                                    path: '/getting-started.qml'
                                    title: 'Getting Started'
                                }
                                ListElement {
                                    path: '/scaffolding.qml'
                                    title: 'Scaffolding'
                                }
//                                ListElement {
//                                    path: '/modules.qml'
//                                    title: 'Modules'
//                                }
                                ListElement {
                                    path: '/examples.qml'
                                    title: 'Examples'
                                }
                            }
                            Component {
                                Bootstrap.NavItem {
                                    _class: http.path === model.path ? 'active' : ''
                                    a.href: model.path
                                    a.text: model.title
                                }
                            }
                        }
                    }
                }
            }
        }

        Bootstrap.Container {
            id: main
            Component.onCompleted: {
                for (var i = 0; i < main.contents.length; i++) {
                    var child = main.contents[i]
                    if (child.tagName === 'head') {
                        child.tagName = ''
                        main.takeAt(i--)
                        head.insert(head.contents.length, child)
                    } else if (child.tagName === 'body') {
                        child.tagName = ''
                        main.takeAt(i--)
                        body.insert(body.contents.length, child)
                    }
                }
            }
        }
        Bootstrap.Navbar {
            _class: 'navbar-fixed-bottom'
//            _class: 'navbar-inverse'
            Bootstrap.Container {
                A {
                    _class: 'brand'
                    href: 'http://silk.qtquick.me/'
                    text: "Copyright (c) 2012 - %1 Silk Project. All rights reserved.".arg(Qt.formatDate(new Date(), 'yyyy'))
                }
            }
        }
        Script { src: '/3rdparty/js/jquery.js' }
        Script { src: '/3rdparty/js/bootstrap.js' }
    //    Script { src: 'http://twitter.github.io/bootstrap/assets/js/jquery.js' }
    //    Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap.js' }
        GoogleAnalytics {
            enabled: http.host === "silk.qtquick.me"
            __trackingCode: 'UA-33461556-1'
        }
    }
}

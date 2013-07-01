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

import Silk.HTML 5.0 as HTML
import Silk.CSS 3.0 as CSS
import Silk.Bootstrap 2.3 as Bootstrap
import Silk.Utils 1.0 as Utils
import QtQml 2.1 as QML
import QtQml.Models 2.1 as Models

HTML.Html {
    lang: 'en'

    HTML.Head {
        id: head
        HTML.Meta { charset: 'utf-8' }
        HTML.Meta { name: 'viewport'; content: 'width=device-width, initial-scale=1.0' }
        HTML.Link { href: 'http://twitter.github.io/bootstrap/assets/css/bootstrap.css'; rel: 'stylesheet' }
        HTML.Link { href: 'http://twitter.github.io/bootstrap/assets/css/bootstrap-responsive.css'; rel: 'stylesheet' }
        HTML.Link { href: 'http://twitter.github.io/bootstrap/assets/css/docs.css'; rel: 'stylesheet' }
        HTML.Link { href: 'http://twitter.github.io/bootstrap/assets/js/google-code-prettify/prettify.css'; rel: 'stylesheet' }
        HTML.Comment { text: ' Le HTML5 shim, for IE6-8 support of HTML5 elements '}
        HTML.Comment { text: '[if lt IE 9]>
      <script src="http://twitter.github.io/bootstrap/assets/js/html5shiv.js"></script>
    <![endif]' }
        HTML.Comment { text: ' Le fav and touch icons ' }
        HTML.Link { rel: 'apple-touch-icon-precomposed'; sizes: '144x144'; href: 'http://twitter.github.io/bootstrap/assets/ico/apple-touch-icon-144-precomposed.png' }
        HTML.Link { rel: 'apple-touch-icon-precomposed'; sizes: '114x114'; href: 'http://twitter.github.io/bootstrap/assets/ico/apple-touch-icon-114-precomposed.png' }
        HTML.Link { rel: 'apple-touch-icon-precomposed'; sizes: '72x72'; href: 'http://twitter.github.io/bootstrap/assets/ico/apple-touch-icon-72-precomposed.png' }
        HTML.Link { rel: 'apple-touch-icon-precomposed'; href: 'http://twitter.github.io/bootstrap/assets/ico/apple-touch-icon-57-precomposed.png' }
        HTML.Link { rel: 'shortcut icon'; href: 'http://twitter.github.io/bootstrap/assets/ico/favicon.png' }
        HTML.Style {
            _id: 'holderjs-style'
            type: 'text/css'
            CSS.Rule {
                selector: '.holderjs-fluid'
                font_size: '16px'
                font_weight: 'bold'
                text_align: 'center'
                font_family: 'sans-serif'
                property string margin: '0'
            }
        }
    }

    HTML.Body {
        id: body
        property string data_spy: 'scroll'
        property string data_target: '.bs-docs-sidebar'
        property string data_twttr_rendered: 'true'

        Bootstrap.Navbar {
            _class: 'navbar-inverse navbar-fixed-top'
            Bootstrap.Container {
                Bootstrap.BtnNavbar {
                    property string data_toggle: 'collapse'
                    property string data_target: '.nav-collapse'
                    Bootstrap.IconBar {}
                    Bootstrap.IconBar {}
                    Bootstrap.IconBar {}
                }
                HTML.A { _class: 'brand'; href: '/'; text: 'Bootstrap' }
                Bootstrap.NavCollapse {
                    Bootstrap.Nav {
                        Utils.Repeater {
                            model: Models.ListModel {
                                Models.ListElement {
                                    href: '/'
                                    text: 'Home'
                                }
                                Models.ListElement {
                                    href: '/getting-started.qml'
                                    text: 'Get started'
                                }
                                Models.ListElement {
                                    href: '/scaffolding.qml'
                                    text: 'Scaffolding'
                                }
                                Models.ListElement {
                                    href: '/base-css.qml'
                                    text: 'Base CSS'
                                }
                                Models.ListElement {
                                    href: '/components.qml'
                                    text: 'Components'
                                }
                                Models.ListElement {
                                    href: '/javascript.qml'
                                    text: 'JavaScript'
                                }
                                Models.ListElement {
                                    href: '/customize.qml'
                                    text: 'Customize'
                                }
                            }

                            QML.Component {
                                Bootstrap.NavItem {
                                    _class: http.path === model.href ? 'active': ''
                                    a.href: model.href
                                    a.text: model.text
                                }
                            }
                        }
                    }
                }
            }
        }

        HTML.Tag {
            id: main
            QML.Component.onCompleted: {
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

        HTML.Footer {
            _class: 'footer'
            Bootstrap.Container {
                HTML.P {
                    text: 'Designed and built with all the love in the world by <a href="http://twitter.com/mdo" target="_blank">@mdo</a> and <a href="http://twitter.com/fat" target="_blank">@fat</a>.'
                }
                HTML.P {
                    text: 'Code licensed under <a href="http://www.apache.org/licenses/LICENSE-2.0" target="_blank">Apache License v2.0</a>, documentation under <a href="http://creativecommons.org/licenses/by/3.0/">CC BY 3.0</a>.'
                }
                HTML.P {
                    text: '<a href="http://glyphicons.com">Glyphicons Free</a> licensed under <a href="http://creativecommons.org/licenses/by/3.0/">CC BY 3.0</a>.'
                }
                HTML.Ul {
                    _class: 'footer-links'
                    Utils.Repeater {
                        model: Models.ListModel {
                            Models.ListElement {
                                href: 'http://blog.getbootstrap.com/'
                                text: 'Blog'
                            }
                            Models.ListElement {
                                href: 'https://github.com/twitter/bootstrap/issues?state=open'
                                text: 'Issues'
                            }
                            Models.ListElement {
                                href: 'https://github.com/twitter/bootstrap/blob/master/CHANGELOG.md'
                                text: 'Changelog'
                            }
                        }

                        QML.Component {
                            HTML.Li {
                                enabled: model.index > 0
                                _class: 'muted'
                                text: ' · '
                            }
                        }
                        QML.Component {
                            HTML.Li {
                                HTML.A {
                                    href: model.href
                                    text: model.text
                                }
                            }
                        }
                    }
                }
            }
        }

        HTML.Comment { text: ' Le javascript
    ================================================== ' }
        HTML.Comment { text: ' Placed at the end of the document so the pages load faster ' }
        HTML.Script { type: 'text/javascript'; src: 'http://platform.twitter.com/widgets.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/jquery.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-transition.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-alert.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-modal.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-dropdown.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-scrollspy.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-tab.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-tooltip.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-popover.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-button.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-collapse.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-carousel.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-typeahead.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-affix.js' }

        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/holder/holder.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/google-code-prettify/prettify.js' }

        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/application.js' }
    }

    default property alias contents: main.contents
}

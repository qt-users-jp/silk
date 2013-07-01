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
import Silk.Bootstrap 2.3 as Bootstrap
import Silk.Utils 1.0
import Silk.Cache 1.0
import QtQml 2.1
import QtQml.Models 2.1

import 'scaffolding/' as Scaffolding

SilkPageTemplate {
    Head {
        Title { text: 'silk - scaffolding' }
    }
    data_spy: 'scroll'
    data_target: '.bs-docs-sidebar'

    Cache { id: cache }

    function loadQml(file, callback) {
        var value = cache.fetch(file)
        if (typeof value !== 'undefined') {
            callback(value)
        } else {
            var request = new XMLHttpRequest()
            request.onreadystatechange = function() {
                switch (request.readyState) {
                case 4: // Done
                    cache.add(file, request.responseText)
                    callback(request.responseText)
                    break
                }
            }
            request.open("GET", file, true)
            request.send()
        }
    }

    Bootstrap.Row {
        Bootstrap.Span3 {
            _class: 'bs-docs-sidebar'
            Bootstrap.NavList {
                _class: 'bs-docs-sidenav affix'
                Repeater {
                    model: ListModel {
                        ListElement {
                            href: '#html'
                            text: 'HTML'
                        }
                        ListElement {
                            href: '#css'
                            text: 'CSS'
                        }
                        ListElement {
                            href: '#json'
                            text: 'JSON'
                        }
                        ListElement {
                            href: '#xml'
                            text: 'XML'
                        }
                        ListElement {
                            href: '#text'
                            text: 'Text'
                        }
                    }

                    Component {
                        Bootstrap.NavItem {
                            a.href: model.href
                            Bootstrap.IconChevronRight {}
                            Text { text: model.text }
                        }
                    }
                }
            }
        }
        Bootstrap.Span9 {
            Section {
                _id: 'html'
                H1 { text: 'HTML' }
                Div {
                    _class: 'bs-docs-example'
                    Bootstrap.Row {
                        Bootstrap.Span4 {
                            H2 { text: 'Source code' }
                            Pre {
                                Component.onCompleted: {
                                    loadQml("scaffolding/HtmlExample.qml", function(ret) { text = ret })
                                }
                            }
                            Bootstrap.ABtnPrimary {
                                _class: 'btn-large'
                                href: './scaffolding/HtmlExample.qml'
                                target: '_blank'
                                text: 'Download source code'
                            }
                        }
                        Bootstrap.Span4 {
                            H2 { text: 'Output' }
                            Pre {
                                Text { text: '&lt;!DOCTYPE html&gt;' }
                                Scaffolding.HtmlExample { property bool escapeHTML: true }
                            }
                            Bootstrap.ABtnPrimary {
                                _class: 'btn-large'
                                href: './scaffolding/html.qml'
                                target: '_blank'
                                text: 'Open output'
                            }
                        }
                    }
                }
            }

            Section {
                _id: 'css'
                H1 { text: 'CSS' }
                Div {
                    _class: 'bs-docs-example'
                    Bootstrap.Row {
                        Bootstrap.Span4 {
                            H2 { text: 'Source code' }
                            Pre {
                                Component.onCompleted: {
                                    loadQml("scaffolding/CssExample.qml", function(ret) { text = ret })
                                }
                            }
                            Bootstrap.ABtnPrimary {
                                _class: 'btn-large'
                                href: './scaffolding/CssExample.qml'
                                target: '_blank'
                                text: 'Download source code'
                            }
                        }
                        Bootstrap.Span4 {
                            H2 { text: 'Output' }
                            Pre {
                                Scaffolding.CssExample { property bool escapeHTML: true }
                            }
                            Bootstrap.ABtnPrimary {
                                _class: 'btn-large'
                                href: './scaffolding/css.qml'
                                target: '_blank'
                                text: 'Open output'
                            }
                        }
                    }
                }
            }

            Section {
                _id: 'json'
                H1 { text: 'JSON' }
                Div {
                    _class: 'bs-docs-example'
                    Bootstrap.Row {
                        Bootstrap.Span4 {
                            H2 { text: 'Source code' }
                            Pre {
                                Component.onCompleted: {
                                    loadQml("scaffolding/JsonExample.qml", function(ret) { text = ret })
                                }
                            }
                            Bootstrap.ABtnPrimary {
                                _class: 'btn-large'
                                href: './scaffolding/JsonExample.qml'
                                target: '_blank'
                                text: 'Download source code'
                            }
                        }
                        Bootstrap.Span4 {
                            H2 { text: 'Output' }
                            Pre {
                                Scaffolding.JsonExample { property bool escapeHTML: true }
                            }
                            Bootstrap.ABtnPrimary {
                                _class: 'btn-large'
                                href: './scaffolding/json.qml'
                                target: '_blank'
                                text: 'Open output'
                            }
                        }
                    }
                }
            }

            Section {
                _id: 'xml'
                H1 { text: 'XML' }
                Div {
                    _class: 'bs-docs-example'
                    Bootstrap.Row {
                        Bootstrap.Span4 {
                            H2 { text: 'Source code' }
                            Pre {
                                Component.onCompleted: {
                                    loadQml("scaffolding/XmlExample.qml", function(ret) { text = ret })
                                }
                            }
                            Bootstrap.ABtnPrimary {
                                _class: 'btn-large'
                                href: './scaffolding/XmlExample.qml'
                                target: '_blank'
                                text: 'Download source code'
                            }
                        }
                        Bootstrap.Span4 {
                            H2 { text: 'Output' }
                            Pre {
                                Text { text: '&lt;?xml version="1.0" encoding="UTF-8" ?&gt;' }
                                Scaffolding.XmlExample { property bool escapeHTML: true }
                            }
                            Bootstrap.ABtnPrimary {
                                _class: 'btn-large'
                                href: './scaffolding/xml.qml'
                                target: '_blank'
                                text: 'Open output'
                            }
                        }
                    }
                }
            }

            Section {
                _id: 'text'
                H1 { text: 'Text' }
                Div {
                    _class: 'bs-docs-example'
                    Bootstrap.Row {
                        Bootstrap.Span4 {
                            H2 { text: 'Source code' }
                            Pre {
                                Component.onCompleted: {
                                    loadQml("scaffolding/TextExample.qml", function(ret) { text = ret })
                                }
                            }
                            Bootstrap.ABtnPrimary {
                                _class: 'btn-large'
                                href: './scaffolding/TextExample.qml'
                                target: '_blank'
                                text: 'Download source code'
                            }
                        }
                        Bootstrap.Span4 {
                            H2 { text: 'Output' }
                            Pre {
                                Scaffolding.TextExample { property bool escapeHTML: true }
                            }
                            Bootstrap.ABtnPrimary {
                                _class: 'btn-large'
                                href: './scaffolding/text.qml'
                                target: '_blank'
                                text: 'Open output'
                            }
                        }
                    }
                }
            }
        }
    }
}

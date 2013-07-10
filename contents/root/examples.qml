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
import Silk.Utils 1.0
import Silk.Bootstrap 2.3 as Bootstrap
import QtQml 2.1
import QtQml.Models 2.1

SilkPageTemplate {
    Head {
        Title { text: 'silk - examples' }
    }
    data_spy: 'scroll'
    data_target: '.bs-docs-sidebar'

    Bootstrap.Row {
        Bootstrap.Span3 {
            _class: 'bs-docs-sidebar'
            Bootstrap.NavList {
                _class: 'bs-docs-sidenav affix'
                Repeater {
                    model: ListModel {
                        ListElement {
                            _id: 'html'
                            title: 'HTML'
                        }
                        ListElement {
                            _id: 'css'
                            title: 'CSS'
                        }
                        ListElement {
                            _id: 'cgi'
                            title: 'CGI'
                        }
                        ListElement {
                            _id: 'database'
                            title: 'Database'
                        }
                        ListElement {
                            _id: 'communication'
                            title: 'Communication'
                        }
                        ListElement {
                            _id: 'advanced'
                            title: 'Advanced'
                        }
                    }

                    Component {
                        Bootstrap.NavItem {
                            a.href: '#%1'.arg(model._id)
                            Bootstrap.IconChevronRight {}
                            Text { text: model.title }
                        }
                    }
                }
            }
        }
        Bootstrap.Span9 {
            Section {
                _id: 'html'
                Bootstrap.PageHeader {
                    H1 { text: 'HTML' }
                }
                Ul {
                    Repeater {
                        model: ListModel {
                            ListElement {
                                example: 'class'
                                files: [
                                    ListElement { source: 'class.qml' }
                                    , ListElement{ source: 'components/Rectangle.qml' }
                                ]
                            }
                            ListElement {
                                example: 'repeater'
                                files: [
                                    ListElement { source: 'repeater.qml' }
                                ]
                            }
                            ListElement {
                                example: 'recursive'
                                files: [
                                    ListElement { source: 'recursive.qml' }
                                ]
                            }
                            ListElement {
                                example: 'json'
                                files: [
                                    ListElement { source: 'json.qml' }
                                    , ListElement { source: 'json.js' }
                                    , ListElement { source: 'jsondata.qml' }
                                ]
                            }
                        }
                        Component {
                            Li {
                                property var __files: model.files
                                A { href: '/examples/%1.qml'.arg(model.example); text: model.example }
                                Ul {
                                    Repeater {
                                        model: __files
                                        Component {
                                            Li {
                                                A { href: 'http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1'.arg(model.source); text: model.source }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Section {
                _id: 'css'
                Bootstrap.PageHeader {
                    H1 { text: 'CSS' }
                }
                Ul {
                    Repeater {
                        model: ListModel {
                            ListElement {
                                example: 'stylesheet1'
                                files: [
                                    ListElement { source: 'stylesheet1.qml' }
                                ]
                            }
                            ListElement {
                                example: 'stylesheet2'
                                files: [
                                    ListElement { source: 'stylesheet2.qml' }
                                    , ListElement{ source: 'css.qml' }
                                ]
                            }
                        }
                        Component {
                            Li {
                                property var __files: model.files
                                A { href: '/examples/%1.qml'.arg(model.example); text: model.example }
                                Ul {
                                    Repeater {
                                        model: __files
                                        Component {
                                            Li {
                                                A { href: 'http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1'.arg(model.source); text: model.source }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Section {
                _id: 'cgi'
                Bootstrap.PageHeader {
                    H1 { text: 'CGI' }
                }
                Ul {
                    Repeater {
                        model: ListModel {
                            ListElement {
                                example: 'header'
                                files: [
                                    ListElement { source: 'header.qml' }
                                ]
                            }
                            ListElement {
                                example: 'get'
                                files: [
                                    ListElement { source: 'get.qml' }
                                ]
                            }
                            ListElement {
                                example: 'post'
                                files: [
                                    ListElement { source: 'post.qml' }
                                ]
                            }
                            ListElement {
                                example: 'upload'
                                files: [
                                    ListElement { source: 'upload.qml' }
                                ]
                            }
                            ListElement {
                                example: 'enabled'
                                files: [
                                    ListElement { source: 'enabled.qml' }
                                ]
                            }
                            ListElement {
                                example: 'cookies'
                                files: [
                                    ListElement { source: 'cookies.qml' }
                                    , ListElement { source: 'components/UserInput.qml' }
                                ]
                            }
                            ListElement {
                                example: 'session'
                                files: [
                                    ListElement { source: 'session.qml' }
                                ]
                            }
                        }
                        Component {
                            Li {
                                property var __files: model.files
                                A { href: '/examples/%1.qml'.arg(model.example); text: model.example }
                                Ul {
                                    Repeater {
                                        model: __files
                                        Component {
                                            Li {
                                                A { href: 'http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1'.arg(model.source); text: model.source }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Section {
                _id: 'database'
                Bootstrap.PageHeader {
                    H1 { text: 'Database' }
                }
                Ul {
                    Repeater {
                        model: ListModel {
                            ListElement {
                                example: 'database'
                                files: [
                                    ListElement { source: 'database.qml' }
                                ]
                            }
                        }
                        Component {
                            Li {
                                property var __files: model.files
                                A { href: '/examples/%1.qml'.arg(model.example); text: model.example }
                                Ul {
                                    Repeater {
                                        model: __files
                                        Component {
                                            Li {
                                                A { href: 'http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1'.arg(model.source); text: model.source }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                Bootstrap.AlertBlock {
                    Text { text: 'Install ' }
                    A { text: "Relational database plugin for QML"; href: 'http://qt5.jp/relational-database-plugin-for-qml.html' }
                    Text { text: " to use database in silk." }
                }
            }
            Section {
                _id: 'communication'
                Bootstrap.PageHeader {
                    H1 { text: 'Communication' }
                }
                Ul {
                    Repeater {
                        model: ListModel {
                            ListElement {
                                example: 'websocket'
                                files: [
                                    ListElement { source: 'websocket.qml' }
                                    , ListElement { source: 'websocket.js' }
                                    , ListElement { source: 'echo.qml' }
                                ]
                            }
                            ListElement {
                                example: 'smtp'
                                files: [
                                    ListElement { source: 'smtp.qml' }
                                ]
                            }
                            ListElement {
                                example: 'oauth'
                                files: [
                                    ListElement { source: 'oauth.qml' }
                                    , ListElement { source: 'components/Twitter.qml' }
                                ]
                            }
                        }
                        Component {
                            Li {
                                property var __files: model.files
                                A { href: '/examples/%1.qml'.arg(model.example); text: model.example }
                                Ul {
                                    Repeater {
                                        model: __files
                                        Component {
                                            Li {
                                                A { href: 'http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1'.arg(model.source); text: model.source }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Section {
                _id: 'advanced'
                Bootstrap.PageHeader {
                    H1 { text: 'Advanced' }
                }
                Ul {
                    Repeater {
                        model: ListModel {
                            ListElement {
                                example: 'timer'
                                files: [
                                    ListElement { source: 'timer.qml' }
                                ]
                            }
                            ListElement {
                                example: 'file'
                                files: [
                                    ListElement { source: 'file.qml' }
                                ]
                            }
                            ListElement {
                                example: 'process'
                                files: [
                                    ListElement { source: 'process.qml' }
                                ]
                            }
                            ListElement {
                                example: 'config'
                                files: [
                                    ListElement { source: 'config.qml' }
                                ]
                            }
                            ListElement {
                                example: 'remote'
                                files: [
                                    ListElement { source: 'remote.qml' }
                                ]
                            }
                            ListElement {
                                example: 'javascript'
                                files: [
                                    ListElement { source: 'javascript.qml' }
                                    , ListElement { source: 'reverse.js' }
                                ]
                            }
                            ListElement {
                                example: 'chat'
                                files: [
                                    ListElement { source: 'chat.qml' }
                                    , ListElement { source: 'chat.js' }
                                    , ListElement { source: 'chatserver.qml' }
                                    , ListElement { source: 'chatdaemon.qml'; path: 'tasks/chatdaemon.qml' }
                                ]
                            }
                        }
                        Component {
                            Li {
                                property var __files: model.files
                                A { href: '/examples/%1.qml'.arg(model.example); text: model.example }
                                Ul {
                                    Repeater {
                                        model: __files
                                        Component {
                                            Li {
                                                A { href: 'http://git.qtquick.me/?p=silk.git;a=blob;f=contents/%1'.arg(model.path ? model.path : 'root/examples/%1'.arg(model.source)); text: model.source }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

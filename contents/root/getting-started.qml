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

SilkPageTemplate {
    Head {
        Title { text: 'silk - getting started' }
    }
    data_spy: 'scroll'
    data_target: '.bs-docs-sidebar'

    Bootstrap.Row {
        Bootstrap.Span3 {
            _class: 'bs-docs-sidebar'
            Bootstrap.NavList {
                _class: 'bs-docs-sidenav affix'
                Bootstrap.NavItem {
                    a.href: '#download-qt'
                    Bootstrap.IconChevronRight {}
                    Text { text: '1. Download Qt' }
                }
                Bootstrap.NavItem {
                    a.href: '#download-silk'
                    Bootstrap.IconChevronRight {}
                    Text { text: '2. Download Silk' }
                }
                Bootstrap.NavItem {
                    a.href: '#configuration'
                    Bootstrap.IconChevronRight {}
                    Text { text: '3. Configuration' }
                }
            }
        }
        Bootstrap.Span9 {
            Section {
                _id: 'download-qt'
                Bootstrap.PageHeader {
                    H1 { text: '1. Download Qt' }
                }
                Bootstrap.Lead {
                    Text { text: 'download and install Qt 5.1 or later from ' }
                    A { href: 'http://qt-project.org/downloads'; target: '_blank'; text: 'qt-project.org' }
                }
            }
            Section {
                _id: 'download-silk'
                Bootstrap.PageHeader {
                    H1 { text: '2. Download Silk' }
                }
                Bootstrap.RowFluid {
                    Bootstrap.Span6 {
                        H2 { text: 'Installer' }
                        Bootstrap.Alert {
                            text: 'Silk installers are not available yet.'
                        }
                    }
                    Bootstrap.Span6 {
                        H2 { text: 'Build from source' }
                        Pre {
                            Kbd { text: '$ git clone <a href="http://git.qtquick.me/?p=silk.git;a=tree" target="_blank">git://git.qtquick.me/silk.git</a>\n' }
                            Kbd { text: "$ cd silk\n" }
                            Kbd { text: "$ git submodule update --init\n" }
                            Kbd { text: "$ qt5/bin/qmake\n" }
                            Kbd { text: "$ make\n" }
                            Kbd { text: "$ ./bin/silk\n" }
                            Text { text: 'silk is running on 8080' }
                        }
                        P {
                            Text { text: 'open ' }
                            A { href: 'http://localhost:8080/'; target: '_blank'; text: 'localhost:8080' }
                        }
                    }
                }
            }
            Section {
                _id: 'configuration'
                Bootstrap.PageHeader {
                    H1 { text: '3. Configuration' }
                }
                Bootstrap.RowFluid {
                    Bootstrap.Span6 {
                        H2 { text: 'Default configuration' }
                        Pre {
                            text: '{
    "listen": {
        "address": "*",
        "port": 8080
    },
    "contents": {
        "*": "../share/silk/root/"
    },
    "silk": {
        "tasks": [
            "../share/silk/tasks/chatdaemon.qml"
        ]
    },
    "storage": {
        "path": "../"
    },
    "import": {
        "path": []
    },
    "cache": {
        "qml": true
    },
    "deflate": {
        "excludes": ["video/*", "image/*"]
    }
}'
                        }
                    }
                    Bootstrap.Span3 {
                        H3 { text: 'listen.address' }
                        Dl {
                            _class: 'well'
                            Dt { text: '"*"' }
                            Dd { text: 'listen any address' }
                            Dt { text: '"localhost"' }
                            Dd { text: 'listen localhost only' }
                            Dt { text: '"nnn.nnn.nnn.nnn"' }
                            Dd { text: 'listen an ip address' }
                        }
                        H3 { text: 'listen.port' }
                        P {
                            _class: 'well'
                            Text { text: 'port number' }
                        }
                        H3 { text: 'contents' }
                        P {
                            _class: 'well'
                            Text { text: 'virtual host name/root path pairs' }
                        }
                        H3 { text: 'silk.tasks' }
                        P {
                            _class: 'well'
                            Text { text: 'background services' }
                        }
                    }
                    Bootstrap.Span3 {
                        H3 { text: 'storage' }
                        P {
                            _class: 'well'
                            Text { text: 'root path for offline storage' }
                        }
                        H3 { text: 'import' }
                        P {
                            _class: 'well'
                            Text { text: 'additional import path for QML' }
                        }
                        H3 { text: 'cache' }
                        P {
                            _class: 'well'
                            Text { text: 'if false, silk generates html from qml everytime' }
                        }
                        H3 { text: 'deflate.excludes' }
                        P {
                            _class: 'well'
                            Text { text: 'mime types not compressed' }
                        }
                    }
                }
                Bootstrap.RowFluid {
                    Bootstrap.Span6 {
                        H2 { text: 'Write your config file' }
                        Pre {
                            text: '{
    "listen": {
        "address": "localhost",
        "port": 8081
    },
    "contents": {
        "*": "/home/silk/contents"
    },
    "silk": {
        "tasks": []
    },
    "storage": {
        "path": "/home/silk/contents"
    }
}'
                        }
                        Bootstrap.AlertInfo {
                            text: 'will override default configuration values'
                        }
                    }
                    Bootstrap.Span6 {
                        H2 { text: 'Run with the config file' }
                        Pre {
                            Kbd { text: "$ ./bin/silk --config mysilkrc" }
                        }
                        Bootstrap.AlertInfo {
                            text: 'Alternatively, you may save it as ~/.silkrc and just run silk'
                        }
                    }
                }
            }
        }
    }

//    Article {
//        Section {
//            H1 { text: "Getting Started" }
//            H2 { text: "1. Requirements" }
//            P {
//                A {
//                    href: "http://qt-project.org/"
//                    Img { src: "./qt-logo.png" }
//                }
//                Text { text: " " }
//                A { href: "http://qt-project.org/downloads"; text: "5.1 or later" }
//            }
//        }

//        Section {
//            H2 { text: "2. Clone source code" }
//        }

//        Section {
//            H2 { text: "3. Build" }
//            Pre {
//            }
//        }

//        Section {
//            H2 { text: "4. Run" }
//            Pre {
//            }
//            P {
//                Text { text: "open " }
//                A { href: 'http://localhost:8080/'; text: 'http://localhost:8080/' }
//            }
//        }
//    }
}

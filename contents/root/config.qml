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

import QtQuick 2.0
import Silk.HTTP 1.1
import Silk.HTML 5.0
import Silk.Cache 1.0

import "./components/"

SilkPageTemplate {
    id: http

    subtitle: "Configuration"
    loading: silkrc.text.length == 0

    Article {
//        Header {
//            H2 { text: "Building Web Contents in a simple language!" }
//        }

        Section {
            H2 { text: "Default configuration values" }
            PlainFile {
                id: silkrc
            }
            Dl {
                Dt { text: "listen.address" }
                Dd {
                    Dl {
                        Dt { text: "*" } Dd { text: "listen any address" }
                        Dt { text: "localhost" } Dd { text: "listen only localhost" }
                        Dt { text: "(any ip address)" } Dd { text: "listen given address" }
                    }
                }
                Dt { text: "listen.port" }
                Dd { text: "lisen port number" }
                Dt { text: "contents" }
                Dd { text: "virtual host name/root path pairs" }
                Dt { text: "storage.path" }
                Dd { text: "root path for offline storage" }
                Dt { text: "import.path" }
                Dd { text: "additional import path for QML" }
                Dt { text: "cache.qml" }
                Dd { text: "if false, silk generats html from qml everytime" }
            }
        }

        Section {
            H2 { text: "Run with your config" }
            PlainFile {
                text: '/* my silkrc */
{
    "listen": { "address": "localhost", "port": 8081 }
    , "contents": { "*": "/home/silk/contents" }
    , "storage": { "path": "/home/silk/storage" }
}'
            }

            Terminal {
                Kbd { text: "$ ./bin/silk --config silkrc" }
            }

            P { text: "Alternatively, you may save it as ~/.silkrc and run" }

            Terminal {
                Kbd { text: "$ ./bin/silk" }
            }
        }

        Footer {
            Nav {
                A { href: "/try.qml"; text: "Try now!" }
                Text { text: " &lt;&lt; Previous / Next &gt;&gt; Tutorial" }
//                A { href: "/config.qml"; text: "Configuration" }
            }
        }
    }

    Cache { id: cache }

    Component.onCompleted: {
        var file = "qrc:/silkrc.default"
        var value = cache.fetch(file)
        if (typeof value !== 'undefined') {
            silkrc.text = value
        } else {
            var request = new XMLHttpRequest()
            request.onreadystatechange = function() {
                switch (request.readyState) {
                case 4: // Done
                    cache.add(file, request.responseText)
                    silkrc.text = request.responseText
                    break
                }
            }
            request.open("GET", file)
            request.send()
        }
    }
}

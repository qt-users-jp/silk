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

    subtitle: "Hello silk world"
    loading: hello_qml.text.length === 0 || hellocss_qml.text.length === 0

    Article {
        Header {
            H2 { text: "This is the hello world in silk" }
        }

        Section {
            H3 { text: "HTML" }
            PlainFile {
                id: hello_qml
            }
            P { text: "silk generates following HTML" }
            PlainFile {
                HelloSource { escape: true }
            }

            ButtonGroup {
                Button {
                    __text: "Output"
                    href: './hello.qml'
                }

                Button {
                    __text: "Source"
                    href: './HelloSource.qml'
                }

                Button {
                    __text: "Details"
                    __disabled: true
//                    href: '#'
                }
            }
        }

        Section {
            H3 { text: "CSS" }
            PlainFile {
                id: hellocss_qml
            }
            P { text: "silk generates following CSS" }
            PlainFile {
                HelloCssSource { escape: true }
            }

            ButtonGroup {
                Button {
                    __text: "Output"
                    href: './hellocss.qml'
                }

                Button {
                    __text: "Source"
                    href: './HelloCssSource.qml'
                }

                Button {
                    __text: "Details"
                    __disabled: true
//                    href: '#'
                }
            }
        }

        Footer {
            Nav {
                A { href: "/"; text: "Introduction" }
                Text { text: " &lt;&lt; Previous / Next &gt;&gt; " }
                A { href: "/try.qml"; text: "Try now!" }
            }
        }
    }

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
            request.open("GET", file)
            request.send()
        }
    }

    Component.onCompleted: {
        loadQml("HelloSource.qml", function(ret) {
            hello_qml.text = ret
        })
        loadQml("HelloCssSource.qml", function(ret) {
            hellocss_qml.text = ret
        })
    }
}

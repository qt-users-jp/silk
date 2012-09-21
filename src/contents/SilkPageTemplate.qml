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

import QtQuick.HTTP 1.1
import QtQuick.HTML 5.0

Http {
    id: root
    status: 200
    responseHeader: {"Content-Type": "text/html; charset=utf-8;"}

    default property alias contents: body.contents

    DocType {}

    Html {
        Head { Title { id: title; text: "silk - a http server for QML" } }

        Body {
            id: body
            Aside {
                Ul {
                    Li { text: "Introduction" }
                    Li { text: "Examples and Demos" }
                    Li {
                        Text { text: "Getting Started" }
                        Ul {
                            Li { text: "Download" }
                            Li { text: "Installation" }
                        }
                    }
                    Li {
                        Text { text: "Tutorials" }
                        Ul {
                            Li { text: "Contents" }
                            Li { text: "Service" }
                        }
                    }
                }
            }

            H1 { text: title.text }

            H2 { text: "Requirements" }
            Ul {
                Li {
                    A { href: "http://qt-project.org/wiki/Qt_5.0"; text: "Qt5" }
                    Ul {
                        Li { text: "QtCore" }
                        Li { text: "QtNetwork" }
                        Li { text: "QtQml" }
                    }
                }
            }

            H2 { text: "Clone source code" }
            Pre { text: "$ git clone git://git.qtquick.me/silk.git" }
            Pre { text: "$ cd silk" }
            Pre { text: "$ git submodule update --init" }

            H2 { text: "Build" }
            Pre { text: "$ qt5/bin/qmake" }
            Pre { text: "$ make" }

            H2 { text: "Run" }
            Pre { text: "$ ./bin/silk" }
            P {
                Text { text: "open&nbsp;" }
                A { href: "http://localhost:8080/"; text: "http://localhost:8080/" }
            }
            H2 { A { href: "./examples/"; text: "Examples" } }
        }
    }
}

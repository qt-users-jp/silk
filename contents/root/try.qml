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

import Silk.HTTP 1.1
import Silk.HTML 5.0
import "./components/"

SilkPageTemplate {
    id: http

    subtitle: "Try now!"

    Article {
//        Header {
//            H2 { text: "Building Web Contents in a simple language!" }
//        }

        Section {
            H2 { text: "Requirements" }
            P {
                A {
                    href: "http://qt-project.org/wiki/Qt_5.0"
                    Img { src: "./qt-logo.png" }
                }
                Text { text: " " }
                A { href: "http://releases.qt-project.org/qt5.0/beta1/"; text: "5.0 beta1" }
                Text { text: " or later" }
            }
        }

        Section {
            H2 { text: "Clone source code" }
            Terminal {
                Kbd { text: '$ git clone <a href="http://git.qtquick.me/?p=silk.git;a=tree">git://git.qtquick.me/silk.git</a>\n' }
                Kbd { text: "$ cd silk\n" }
                Kbd { text: "$ git submodule update --init" }
            }
        }

        Section {
            H2 { text: "Build" }
            Terminal {
                Kbd { text: "$ qt5/bin/qmake\n" }
                Kbd { text: "$ make" }
            }
        }

        Section {
            H2 { text: "Run" }
            Terminal {
                Kbd { text: "$ ./bin/silk" }
            }
            P {
                Text { text: "open http://localhost:8080/" }
            }
        }

        Footer {
            Nav {
                A { href: "/features.qml"; text: "Features" }
                Text { text: " &lt;&lt; Previous / Next &gt;&gt; " }
                A { href: "/config.qml"; text: "Configuration" }
            }
        }
    }
}

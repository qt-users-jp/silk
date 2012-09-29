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

    subtitle: "Features"

    Article {
//        Header {
//            H2 { text: "Building Web Contents in a simple language!" }
//        }

        Section {
            H2 { text: "Standards" }
            Table {
                border: "1"
                property string style: "margin-bottom: 10px;"
                THead {
                    Tr {
                        Th { text: "Feature" }
                        Th { text: "Module" }
                        Th { text: "Specification" }
                    }
                }
                TBody {
                    Tr {
                        Td { text: "HTTP" }
                        Td { text: "Silk.HTTP 1.1" }
                        Td { A { href: "http://www.ietf.org/rfc/rfc2616.txt"; text: "HTTP/1.1" } }
                    }
                    Tr {
                        Td { text: "HTML"; rowspan: "2" }
                        Td { text: "Silk.HTML 4.01" }
                        Td { A { href: "http://www.w3.org/TR/html401/"; text: "HTML 4.01" } }
                    }
                    Tr {
                        Td { text: "Silk.HTML 5.0" }
                        Td { A { href: "http://www.w3.org/TR/html5/"; text: "HTML5" } }
                    }
                    Tr {
                        Td { text: "CSS"; rowspan: "2" }
                        Td { text: "Silk.CSS 2.1" }
                        Td { A { href: "http://www.w3.org/TR/CSS21/"; text: "CSS 2.1" } }
                    }
                    Tr {
                        Td { text: "Silk.CSS 3.0" }
                        Td { text: "&nbsp;" }
                    }
                    Tr {
                        Td { text: "JSON" }
                        Td { text: "Silk.JSON 1.0" }
                        Td { A { href: "http://www.ietf.org/rfc/rfc4627.txt"; text: "JSON" } }
                    }
                    Tr {
                        Td { text: "RDB" }
                        Td { text: "Silk.Database 1.0" }
                        Td { text: "&nbsp;" }
                    }
                    Tr {
                        Td { text: "SMTP" }
                        Td { text: "Silk.SMTP 1.0" }
                        Td { A { href: "http://www.ietf.org/rfc/rfc5321.txt"; text: "SMTP" } }
                    }
                    Tr {
                        Td { text: "OAuth" }
                        Td { text: "Silk.OAuth 1.0" }
                        Td { A { href: "http://www.ietf.org/rfc/rfc5849.txt"; text: "OAuth 1.0" } }
                    }
                }
            }
        }

        Section {
            H2 { text: "Utilities" }
            Table {
                border: "1"
                property string style: "margin-bottom: 10px;"
                THead {
                    Tr {
                        Th { text: "Utilitiy" }
                        Th { text: "Module" }
                        Th { text: "Description" }
                    }
                }
                TBody {
                    Tr {
                        Td { text: "Cache" }
                        Td { text: "Silk.Cache 1.0" }
                        Td { text: "Global variant cache for QML" }
                    }
                    Tr {
                        Td { text: "Process" }
                        Td { text: "Silk.Process 1.0" }
                        Td { text: "process programs on server" }
                    }
                    Tr {
                        Td { text: "Utils" }
                        Td { text: "Silk.Utils 1.0" }
                        Td { text: "for Repeater element" }
                    }
                }
            }
        }

        Footer {
            Nav {
                A { href: "/hellosilk.qml"; text: "Hello silk world" }
                Text { text: " &lt;&lt; Previous / Next &gt;&gt; " }
                A { href: "/try.qml"; text: "Try now!" }
            }
        }
    }
}

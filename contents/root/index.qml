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
    id: page

    subtitle: "a simple web framework"

    Article {
        Section {
            H2 { text: "How many languages are you <em>writing</em> to develop a web service?" }
            Table {
                property string style: "width: 100%; border-collapse: separate; border-spacing: 5px; table-layout: fixed;"
                Tr {
                    ArchitectureBlock { text: "HTML"; rowspan: "2"; __class: "arch_html arch_op50" }
                    ArchitectureBlock { text: "HTML"; colspan: "2"; __class: "arch_html arch_op50" }
                    ArchitectureBlock { text: "XML / JSON"; colspan: "2"; __class: "arch_js arch_op50" }
                    ArchitectureBlock { text: "CSS"; rowspan: "2"; __class: "arch_css" }
                    ArchitectureBlock { text: "Java<br />Script"; rowspan: "2"; __class: "arch_js" }
                }
                Tr {
                    ArchitectureBlock {
                        text: "Ruby / PHP / Java / ..."
                        colspan: "4"
                        __class: "arch_plangs"
                    }
                }
                Tr {
                    ArchitectureBlock {
                        text: "Apache / IIS / nginx / ..."
                        colspan: "7"
                        __class: "arch_server arch_op50"
                    }
                }
            }
            Strong { text: "You may have to write them in:" }
            Ol {
                Li { text: "Ruby / PHP / Java / ..." }
                Li { text: "JavaScript" }
                Li { text: "(S)CSS" }
                Li { text: "(HTML)" }
                Li { text: "..." }
            }
            Strong { text: "Seems like something should be improved. so..." }
        }

        Section {
            H2 { text: "We did it!" }
            Table {
                property string style: "width: 100%; border-collapse: separate; border-spacing: 5px; table-layout: fixed;"
                Tr {
                    ArchitectureBlock { text: "HTML"; rowspan: "2"; __class: "arch_html arch_op50" }
                    ArchitectureBlock { text: "HTML"; colspan: "2"; __class: "arch_html arch_op50" }
                    ArchitectureBlock { text: "JSON"; colspan: "2"; __class: "arch_js arch_op50" }
                    ArchitectureBlock { text: "CSS"; __class: "arch_css arch_op50" }
                    ArchitectureBlock { text: "Java<br />Script"; rowspan: "2"; __class: "arch_js" }
                }
                Tr {
                    ArchitectureBlock {
                        text: "QML with JavaScript"
                        colspan: "5"
                        __class: "arch_qml"
                    }
                }
                Tr {
                    ArchitectureBlock {
                        text: "Silk"
                        colspan: "7"
                        __class: "arch_qt arch_op50"
                    }
                }
            }
            Strong { text: "You are able to write everything in:" }
            Ol {
                Li { text: "QML and JavaScript" }
                Li { text: "(HTML)" }
            }
        }

        Footer {
            Nav {
                Text { text: "Next &gt;&gt; " }
                A { href: "/hellosilk.qml"; text: "Hello silk world" }
            }
        }
    }
}

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
import QtQuick.HTML 4.01
import QtQuick.CSS 2.1

Http {
    id: root
    status: 200
    responseHeader: {'Content-Type': root.mode != "css" ? 'text/html; charset=utf-8;' : 'text/css;'}

    property string mode: typeof root.query !== 'undefined' && root.query.indexOf("=") > 0 ? decodeURIComponent(root.query.substring(root.query.indexOf("=") + 1).replace('+', ' ')) : ''

    Html {
        enabled: root.mode != "css"
        Head {
            Title { id: title; text: "Creating Single Element iPhone using CSS3" }
            Link { rel: "stylesheet"; type: "text/css"; href: "%1?mode=%2".arg(root.url).arg("css") }
        }

        Body {
            H1 { A { href: "http://cssdeck.com/labs/creating-single-element-iphone-using-css3"; text: title.text } }
            Div { _class: "iphone" }
        }
    }

    Rule {
        enabled: root.mode == "css"
        selector: "body"
        background: "#111"

        Rule {
            selector: " a"

            // rules without selector work for parent
            Rule {
                color: "#FFFFFF"
            }
        }

        IPhoneCSS {
            selector: " .iphone"
        }
    }
}

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
import 'reverse.js' as Script

Http {
    id: root
    status: 200
    responseHeader: {'Content-Type': 'text/html; charset=utf-8;'}

    property string source: typeof root.data !== 'undefined' && root.data.indexOf("=") > 0 ? decodeURIComponent(root.data.substring(root.data.indexOf("=") + 1).replace('+', ' ')) : ''

    Html {
        Head {
            Title { id: title; text: "javascript" }
            Script { type: "text/javascript"; src: "./reverse.js" }
        }

        Body {
            H1 { text: title.text }

            Form {
                action: "./javascript.qml"
                method: "POST"
                Input { id: source; _id: "source"; type: "text"; name: "source"; value: root.source.length > 0 ? root.source : "QML" }
                Input { type: "submit"; value: "Exec on server side" }
                Button { text: "Exec on client side"; onclick: "document.getElementById('result').innerHTML = reverse(document.getElementById('source').value); return false;" }
                P { id: result; _id: "result"; text: Script.reverse(root.source) }
            }
        }
    }
}
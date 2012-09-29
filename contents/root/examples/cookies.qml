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
import Silk.HTML 4.01
import Silk.Utils 1.0
import "./components"

Http {
    id: http
    status: 200
    responseHeader: {'Content-Type': 'text/html; charset=utf-8;'}

    property var cookies: http.requestCookies
    UserInput {
        id: input

        property string action
        property string name
        property string value

        onSubmit: {
            var cookies = http.cookies;
            var responseCookies = http.responseCookies;
            switch(input.action) {
            case 'remove':
                responseCookies[input.name] = {value: undefined, expires: new Date(0)};
                delete cookies[input.name]
                break
            case 'set':
                responseCookies[input.name] = {value: input.value};
                var cookie = cookies[input.name];
                if (typeof cookie !== 'undefined')
                    cookie.value = input.value;
                else
                    cookie = {value: input.value};
                cookies[input.name] = cookie;
                break
            }
            http.responseCookies = responseCookies;
            http.cookies = cookies;
        }
    }

    Html {
        Head {
            Title { id: title; text: "Cookies" }
        }

        Body {
            H1 { text: title.text }

            Dl {
                Repeater {
                    model: http.cookies
                    Component {
                        Dt { id: name; text: model.key.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;') }
                    }
                    Component {
                        Dd {
                            Form {
                                method: "POST"
                                Input { type: "hidden"; name: "action"; value: "remove" }
                                Input { type: "hidden"; name: "name"; value: model.key.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;') }
                                Text { text: model.value.value.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;') }
                                Br {}
                                Input { type: "submit"; value: "Remove" }
                            }
                        }
                    }
                }
            }

            Form {
                action: http.path
                method: "POST"
                Input { type: "text"; name: "name" }
                Text { text: '=' }
                Input { type: "text"; name: "value" }
                Input { type: "hidden"; name: "action"; value: "set" }
                Input { type: "submit"; value: "Add/Update" }
            }
        }
    }
}

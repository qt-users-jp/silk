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
import "./components"

Html {
    UserInput {
        id: input
        property string action
        property string username
        property string password
        property string expires
        property bool loggedIn: input.username === 'test' && input.password === 'test'

        onSubmit: {
            var responseCookies = http.responseCookies;
            switch(input.action) {
            case 'login': {
                if (input.loggedIn) {
                    var cookie = {};
                    cookie.value = 'logged in';
                    if (input.expires === 'week') {
                        var expires = new Date();
                        expires.setTime(expires.getTime() + 7 * 86400000);
                        cookie.expires = expires;
                    }
                    cookie.httponly = true
                    responseCookies['silk-session'] = cookie;
                }

                break; }
            case 'logout':
                responseCookies['silk-session'] = {value: undefined, expires: new Date(0)};
                break;
            default:
                if (typeof http.requestCookies['silk-session'] !== 'undefined' && http.requestCookies['silk-session'].value === 'logged in') {
                    input.username = 'test'
                    input.password = 'test'
                }
                break;
            }
            http.responseCookies = responseCookies;
        }
    }

    Head {
        Title { id: title; text: "Session" }
    }

    Body {
        H1 { text: title.text }

        Form {
            enabled: !input.loggedIn
            action: http.path
            method: "POST"
            Dl {
                Dt { text: "Test account" }
                Dd { text: "test/test" }
                Dt { text: "User:" }
                Dd {
                    Input { type: "text"; name: "username"; value: input.username }
                }
                Dt { text: "Password:" }
                Dd {
                    Input { type: "password"; name: "password"; value: input.password }
                }
                Dt { text: "Keep me logged in for a week:" }
                Dd {
                    Input { type: "checkbox"; name: "expires"; value: "week" }
                }
            }

            Input { type: "hidden"; name: "action"; value: "login" }
            Input { type: "submit"; value: "Login" }
        }

        Form {
            enabled: input.loggedIn
            action: http.path
            method: "POST"
            Dl {
                Dt { text: "User:" }
                Dd {
                    Text { text: input.username }
                }
            }

            Input { type: "hidden"; name: "action"; value: "logout" }
            Input { type: "submit"; value: "Logout" }
        }
    }
}

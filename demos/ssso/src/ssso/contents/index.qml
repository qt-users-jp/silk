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
import Silk.Utils 1.0

SSSOBase {
    id: http

    property string username_error
    property string password_error

    SilkConfig {
        id: config
        property variant ssso
    }

    UserInput {
        id: input

        property string action
        property string username
        property string password
        property string password2
        property string email
        property string key
        property string redirect: typeof http.requestCookies !== 'undefined' && typeof http.requestCookies.ssso_from !== 'undefined' ? http.requestCookies.ssso_from.value : ''

        onSubmit: {
            switch (input.action) {
            case 'signin':
                for (var i = 0; i < accountModel.count; i++) {
                    var account = accountModel.get(i);
                    if (account.username === input.username && account.password === input.password) {
                        var session_id = Silk.uuid();
                        sessionModel.insert({'key': session_id, 'account_id': account.id})
                        http.status = 302
                        var cookies = http.requestCookies;
                        cookies.ssso_session_id = {'value': session_id, 'domain': config.ssso.domain, 'path': '/'}
                        cookies.ssso_from = {'value': undefined, 'domain': config.ssso.domain, 'path': '/', 'expires': new Date(0)}
                        http.responseCookies = cookies;
                        console.debug('Location', input.redirect);
                        http.responseHeader = {"Location": input.redirect};
                        return;
                    }
                }
                http.username_error = 'or';
                http.password_error = 'is wrong.';
                break;
            case 'signout':
                sessionModel.remove({'key': http.requestCookies.ssso_session_id.value})
                var cookies = http.requestCookies;
                cookies.ssso_session_id = {'value': session_id, 'domain': config.ssso.domain, 'path': '/', 'expires': new Date(0)}
                http.responseCookies = cookies;
                break;
            default:
                if (typeof http.requestCookies.ssso_session_id !== 'undefined') {
                    setAccount(http.requestCookies.ssso_session_id.value);
                }
                break;
            }
        }
    }

    function setAccount(key) {
        for (var i = 0; i < sessionModel.count; i++) {
            var session = sessionModel.get(i);
            if (session.key === key) {
                for (var j = 0; j < accountModel.count; j++) {
                    var account = accountModel.get(j);
                    if (account.id === session.account_id) {
                        http.account = account;
                        break;
                    }
                }
                break;
            }
        }
    }

    SSSODatabase {
        AccountModel { id: accountModel }
        SessionModel { id: sessionModel }
    }

    DocType { enabled: http.status === 200 }

    Html {
        enabled: http.status === 200
        Head {
            Title { id: title; text: "Simple Single Sign On" }
        }

        Body {
            Header {
                H1 { text: title.text }
            }
            Section {
                Form {
                    enabled: http.account.username.length === 0
                    method: 'POST'
                    Dl {
                        Dt { text: 'Username:' }
                        Dd {
                            Input { type: 'text'; name: 'username'; value: input.username }
                            Text {
                                enabled: http.username_error.length > 0
                                text: http.username_error
                            }
                        }
                        Dt { text: 'Password:' }
                        Dd {
                            Input { type: 'password'; name: 'password'; value: input.password }
                            Text {
                                enabled: http.password_error.length > 0
                                text: http.password_error
                            }
                        }
                    }

                    Input { type: 'hidden'; name: 'action'; value: 'signin' }
                    Input { type: 'hidden'; name: 'redirect'; value: input.redirect }
                    Input { type: 'submit'; value: 'Sign in' }
                }
                Form {
                    id: signout
                    enabled: http.account.username.length > 0
                    method: 'POST'
                    Dl {
                        Dt { text: 'Username:' }
                        Dd {
                            Text { text: http.account.username }
                        }
                    }

                    Input { type: 'hidden'; name: 'action'; value: 'signout' }
                    Input { type: 'submit'; value: 'Sign out' }
                }
            }
            Footer {
                enabled: http.account.username.length === 0
                A { href: '/signup.qml'; text: 'Sign up' }
            }
        }
    }
}

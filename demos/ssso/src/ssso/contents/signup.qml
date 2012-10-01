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
import Silk.SMTP 1.0
import Silk.Utils 1.0

SSSOBase {
    id: http

    property string username_error
    property string email_error

    UserInput {
        id: input

        property string action
        property string username
        property string email
        property string redirect: typeof http.requestCookies !== 'undefined' && typeof http.requestCookies.ssso_from !== 'undefined' ? http.requestCookies.ssso_from.value : ''

        onSubmit: {
            switch (input.action) {
            case 'signup':
                if (!isError()) {
                    sendMail();
                }
                break;
            }
        }
    }

    function isError() {
        var ret = false;
        if (input.username.match(/^[a-zA-Z0-9_]+$/)) {
            for (var i = 0; i < accountModel.count; i++) {
                var account = accountModel.get(i);
                if (account.username === input.username) {
                    http.username_error = 'is already taken.';
                    ret = true;
                    break;
                }
            }
        } else {
            http.username_error = 'is invalid.';
            ret = true;
        }

        var email = new RegExp('^[0-9a-zA-Z][0-9a-zA-Z_\\.\\-\\+]*@[0-9a-zA-Z][0-9a-zA-Z_\\.\\-]*\\.[a-z]+$');
        if (!input.email.match(email)) {
            http.email_error = "is not valid.";
            ret = true;
        }
        return ret;
    }

    function sendMail() {
        var account = {};
        account.username = input.username;
        account.email = input.email;
        account.signedup = new Date();
        var id = accountModel.insert(account);

        var key = Silk.uuid();
        var signup = { 'key': key };
        signup.account_id = id;
        signup.redirect = input.redirect;
        signup.remote_addr = http.remoteAddress;
        if (typeof http.requestHeader.user_agent !== 'undefined')
            signup.user_agent = http.requestHeader.user_agent;
        signupModel.insert(signup);

        var email = { 'to': [input.email] };
        email.subject = 'qtquick.me account';
        email.body = 'Hi %1,\r\n\r\nThanks for signing up an account in qtquick.me.\r\nPlease visit %3/password.qml?account=%2'.arg(input.username).arg(key).arg(config.ssso.loginUrl);
        http.loading = true;
        smtp.send(email);
    }

    SSSODatabase {
        AccountModel { id: accountModel }
        SignupModel { id: signupModel }
    }

    SilkConfig {
        id: config
        property variant email: {'host': '', 'encrypt': '', 'port': 0, 'username': '', 'password': '', 'from': '' }
        property variant ssso: {'loginUrl': '' }
    }

    Smtp {
        id: smtp
        host: config.email.host
        encrypt: config.email.encrypt
        port: config.email.port
        username: config.email.username
        password: config.email.password
        from: config.email.from
        onSent: {
            sent.text = 'sent an email to %1'.arg(input.email);
            http.loading = false;
        }

        onError: {
            http.loading = false;
        }
    }

    DocType {}

    Html {
        Head {
            Title { id: title; text: "Simple Single Sign On" }
        }

        Body {
            Header {
                H1 { text: title.text }
            }
            Section {
                Form {
                    enabled: sent.text.length == 0
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
                        Dt { text: 'Email:' }
                        Dd {
                            Input { type: 'email'; name: 'email'; value: input.email }
                            Text {
                                enabled: http.email_error.length > 0
                                text: http.email_error
                            }
                        }
                    }

                    Input { type: 'hidden'; name: 'action'; value: 'signup' }
                    Input { type: 'hidden'; name: 'redirect'; value: input.redirect }
                    Input { type: 'submit'; value: 'Sign up' }
                }
                P {
                    id: sent
                    enabled: text.length > 0
                }
            }
            Footer {
                A { href: '/'; text: 'Sign in' }
            }
        }
    }
}

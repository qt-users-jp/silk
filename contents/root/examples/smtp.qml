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
import Silk.HTML 4.01
import Silk.Utils 1.0
import Silk.SMTP 1.0
import "./components"

Http {
    id: http
    status: 200
    responseHeader: {'Content-Type': 'text/html; charset=utf-8;'}

    UserInput {
        onDataChanged: {
            switch(data.action) {
            case 'send':
                http.loading = true;
                smtp.send({'to': [decodeURIComponent(data.email)], 'subject': 'smtp test mail', 'body': 'sent in http://silk.qtquick.me/examples/smtp.qml'});
                break;
            default:
                break;
            }
        }
    }

    SilkConfig { id: config }

    Smtp {
        id: smtp
        host: typeof config.data.email !== 'undefined' ? config.data.email.host : ''
        encrypt: typeof config.data.email !== 'undefined' ? config.data.email.encrypt : ''
        port: typeof config.data.email !== 'undefined' ? config.data.email.port : 0
        username: typeof config.data.email !== 'undefined' ? config.data.email.username : ''
        password: typeof config.data.email !== 'undefined' ? config.data.email.password : ''
        from: typeof config.data.email !== 'undefined' ? config.data.email.from : ''
        onSent: {
            msg.text = 'Sent to %1'.arg(email.to);
            form.enabled = false;
            http.loading = false;
        }
        onError: {
            msg.text = message;
            form.enabled = false;
            http.loading = false;
        }
    }

    Html {
        Head {
            Title { id: title; text: "Send email" }
        }

        Body {
            H1 { text: title.text }
            P {
                enabled: smtp.host.length === 0
                Text {
                    text: 'to test smtp.qml you have to add email config to <a href="/config.qml">silkrc</a> like below.'
                }
                Pre {
                    text: '{
    "email": {
        "host": "<em>smtp server address</em>"
        , "encrypt": "<em>SSL</em>"
        , "port": <em>465</em>
        , "username": "<em>username</em>"
        , "password": "<em>password</em>"
        , "from": "<em>your mail address</em>"
    }
}'
                }
            }
            P {
                Text { id: msg }
            }
            Form {
                id: form
                enabled: smtp.host.length > 0
                method: 'POST'
                Text { text: "e-mail address: " }
                Input { type: 'text'; name: 'email' }
                Input { type: 'submit'; value: 'Send' }
                Input { type: 'hidden'; name: 'action'; value: 'send' }
            }
        }
    }
}

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
import Silk.HTML 5.0
import Silk.Utils 1.0
import "./components"

Http {
    id: http

    Client {
        id: client
        connectionName: 'websocketChatServer'
        onRespound: messages.model = message.messages;
    }

    onReady: client.request({"action": "messages"}, client);

    SilkConfig {
        id: config
        property variant websocket
    }

    Html {
        Head {
            Title { id: title; text: "chat" }
            Script { type: "text/javascript"; src: "./chat.js" }
        }

        Body {
            onload: "connect('ws://%1:%2/examples/chatserver.qml')".arg(typeof config.websocket.host !== 'undefined' ? config.websocket.host : http.host).arg(typeof config.websocket.port !== 'undefined' ? config.websocket.port : http.port)
            H1 { text: title.text }

            Form {
                onsubmit: "post(); return false;"

                Dl {
                    _id: "dl"

                    Dt { Input { _id: "name"; type: "text"; name: "name"; size: '10'; placeholder: "Name" } }
                    Dd {
                        _id: "input"
                        Input { _id: "message"; type: "text"; name: "message"; size: '30'; placeholder: "Message..." }
                        Input { type: "submit"; value: "Send" }
                    }

                    Repeater {
                        id: messages
                        model: []
                        Component {
                            Dt { text: model.user.replace(/&/g, '&amp;') }
                        }
                        Component {
                            Dd { text: model.message.replace(/&/g, '&amp;') }
                        }
                    }
                }
            }
        }
    }
}

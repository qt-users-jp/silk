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
import Silk.Database 1.0
import "./components"

Http {
    id: http
    status: 200
    responseHeader: {'Content-Type': 'text/html; charset=utf-8;'}

    UserInput {
        id: input

        property string action
        property variant keys
        property string value

        onSubmit: {
            switch(input.action) {
            case 'remove':
                for (var i in data) {
                    if (i.substring(0, 'key_'.length) === 'key_') {
                        chat.remove({'key': i.substring('key_'.length)});
                    }
                }
                break;
            case 'insert':
                chat.insert({'value': input.value.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')});
                break;
            default:
                break;
            }
        }
    }

    Database {
        connectionName: 'examples/database.qml'
        type: "QSQLITE"
        databaseName: ":memory:"

        TableModel {
            id: chat
            name: 'Chat'
            primaryKey: 'key'
            property int key
            property string value
        }
    }


    Html {
        Head {
            Title { id: title; text: "Sql Database" }
        }

        Body {
            H1 { text: title.text }
            Form {
                method: 'POST'
                Text { text: "value: " }
                Input { type: 'text'; name: 'value' }
                Input { type: 'submit'; value: 'Insert' }
                Input { type: 'hidden'; name: 'action'; value: 'insert' }
            }
            Form {
                method: 'POST'
                Table {
                    THead {
                        Tr {
                            Th { text: '&nbsp;' }
                            Th { text: 'key' }
                            Th { text: 'value' }
                        }
                    }
                    TBody {
                        Repeater {
                            model: chat
                            Tr {
                                Td { Input { type: 'checkbox'; name: typeof model !== 'undefined' && typeof model.key !== 'undefined' ? 'key_%1'.arg(model.key) : '' } }
                                Td { text: typeof model !== 'undefined' && typeof model.key !== 'undefined' ? model.key : '' }
                                Td { text: typeof model !== 'undefined' && typeof model.value !== 'undefined' ? model.value : '' }
                            }
                        }
                    }
                    TFoot {
                        Tr {
                            Td {
                                colspan: '3'
                                Input { type: 'submit'; value: 'Remove checked data' }
                                Input { type: 'hidden'; name: 'action'; value: 'remove' }
                            }
                        }
                    }
                }
            }
        }
    }
}

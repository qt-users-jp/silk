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
import Silk.JSON 1.0
import Silk.Utils 1.0
import QtQuick.LocalStorage 2.0

Http {
    id: root
    status: 200
    responseHeader: {'Content-Type': root.post ? 'text/json; charset=utf-8;' : 'text/html; charset=utf-8;'}

    property bool post: root.data.length > 0

    Html {
        id: html
        enabled: !root.post
        Head {
            Title { id: title; text: "chat" }
            Script { type: "text/javascript"; src: "./chat.js" }
        }

        Body {
            onload: "document.getElementById('message').focus()"
            H1 { text: title.text }

            Form {
                onsubmit: "post(document.getElementById('message')); return false;"
                Input { _id: "message"; type: "text"; name: "message" }
                Button { text: "POST"; onclick: "post(document.getElementById('message')); return false;" }
            }

            Ul {
                id: ul
                _id: "ul"
                Repeater {
                    id: lis
                    model: []
                    Component {
                        Li {
                            title: model.id
                            text: model.message.replace(/&/g, '&amp;')
                        }
                    }
                }
            }
        }
    }

    Json {
        id: json
        enabled: root.post
    }

    onReady: {
        var ret = []
        var db = LocalStorage.openDatabaseSync("chat.qml", "1.0", "chat", 1000000);
        var where = ''
        var hasMessage = false;

        if (root.post) {
            var data = JSON.parse(root.data)
            if (typeof data.message !== 'undefined') {
                hasMessage = true;

                db.transaction (
                            function (tx) {
                                tx.executeSql('CREATE TABLE IF NOT EXISTS Chat(id INTEGER PRIMARY KEY AUTOINCREMENT, message TEXT)');
                                tx.executeSql('INSERT INTO Chat(message) VALUES(?)', [data.message]);
                            }
                            )
            }
            if (typeof data.since !== 'undefined') {
                where = ' WHERE id > %1'.arg(data.since)
            }
        }

        try {
            db.readTransaction (
                        function (tx) {
                            var sql = 'SELECT * FROM Chat%1 ORDER BY id DESC LIMIT 100';
                            var rs = tx.executeSql(sql.arg(where));
                            for (var i = 0; i < rs.rows.length; i++) {
                                var item = rs.rows.item(i);
                                ret.push(item);
                            }
                        }
                        )
        } catch (e) {}
        if (root.post) {
            json.object = ret;
        } else {
            lis.model = ret;
        }
    }
}

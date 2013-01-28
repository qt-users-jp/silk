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

import QtQml 2.0
import Silk.HTML 5.0
import Silk.Utils 1.0
import Silk.MongoDB 0.1

Html {

    Database {
        id: db
        host: config.mongodb.host
        port: config.mongodb.port
        name: config.mongodb.name
        user: config.mongodb.user
        pass: config.mongodb.pass

        property Collection test: Collection { name: 'test' }
    }


    Head {
        Title { id: title; text: "MongoDB" }
    }

    Body {
        H1 { text: title.text }
        H2 { text: 'model: db.test.find()' }
        Ul {
            Repeater {
                model: db.test.find()
                Component {
                    Li { text: "{data: %1}".arg(model.data) }
                }
            }
        }
    }

    SilkConfig {
        id: config
        property var mongodb: {'host': '127.0.0.1', 'port': 27017, 'name': 'test', 'user': '', 'pass': ''}
    }
}

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
import Silk.Database 1.0
import Silk.RSS 2.0
import Silk.Utils 1.0

Rss {
    id: root

    SilkConfig {
        id: config
        property variant blog: {author: 'task_jp'; database: ':memory:'; title: 'Qt { version: 5 }'}
        property variant contents: {}
    }

    Database {
        id: db
        connectionName: 'blog'
        type: "QSQLITE"
        databaseName: config.blog.database
    }

    ArticleModel {
        id: articleModel
        database: db
        select: true
        condition: 'published <> ""'
        limit: 20
    }

    QtObject {
        id: viewer
        property var plugins

        Component.onCompleted: {
            var plugins = {}
            var dir = config.contents['*'] + 'plugins/'
            var files = Silk.readDir(dir)
            for (var i = 0; i < files.length; i++) {
                var component = Qt.createComponent(dir + files[i])
                switch (component.status) {
                case Component.Ready: {
                    var plugin = component.createObject(viewer, {config: config})
                    plugins[plugin.name] = plugin.exec
                    break }
                case Component.Error:
                    console.debug(component.errorString())
                    break
                default:
                    console.debug(component.status)
                    break
                }
            }
            viewer.plugins = plugins
        }

        function show(html, no) {
            html = html.replace(/plugin/g, '\v')
            while (html.match(/<\v type=\"([^"]+)\" argument=\"([^"]+)\">([^\v]*?)<\/\v>/)) {
                html = html.replace(/<\v type=\"([^"]+)\" argument=\"([^"]+)\">([^\v]*?)<\/\v>/gm, function(str, plugin, argument, innerText) {
                    var ret = str
                    if (typeof viewer.plugins[plugin] === 'undefined') {
                        console.debug('plugin %1 not found.'.arg(plugin))
                        ret = innerText
                    } else {
                        ret = (viewer.plugins[plugin])(argument.replace('[id]', no), innerText, true)
                    }
                    return ret
                })
            }
            html = html.replace(/\v/g, 'plugin')
            return html
        }
    }

    function escapeHTML(str) {
        return str.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')
    }

    Channel {
        Title { text: 'Test' }
        Link { text: 'http://%1%2'.arg(http.host).arg(http.port === 80 ? '' : ':' + http.port) }
        Language { text: 'ja' }
        PubDate { text: (new Date()).toUTCString() }
        LastBuildDate { text:articleModel.count > 0 ? articleModel.get(0).published.toUTCString() : '' }

        Repeater {
            model: articleModel
            Component {
                Item {
                    Title { text: root.escapeHTML(model.title) }
                    Link { text: 'http://%1%2/?no=%3'.arg(http.host).arg(http.port === 80 ? '' : ':' + http.port).arg(model.id) }
                    Description { text: root.escapeHTML(viewer.show(model.body, model.id)) }
                    PubDate { text: model.published.toUTCString() }
                    Guid { text: 'http://%1%2/?no=%3'.arg(http.host).arg(http.port === 80 ? '' : ':' + http.port).arg(model.id); isPermaLink: 'false' }
                }
            }
        }
    }
}

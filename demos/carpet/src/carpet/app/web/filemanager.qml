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
import Carpet 1.0
import "./components/"
import 'mimetype.js' as MimeType

CarpetTemplate {
    id: html

    UserInput {
        id: input
        property bool ready: false
        property int backup: -1
        property string path: '/'

        onSubmit: {
            input.ready = true
            pathModel.update()

            var path = '/'
            var directories = []
            var file = {path: '/', name: '', mime: 'inode/directory'}
            for (var i = 0; i < pathModel.count(); i++) {
                file = pathModel.get(i)
                if (file.mime === 'inode/directory') {
                    directories.push(file)
                }
            }
            client.directories = directories
            client.file = file

            entryModel.update()
            historyModel.update()
        }
    }

    PathModel {
        id: pathModel
        path: input.path
    }

    EntryModel {
        id: entryModel
        path: client.directories.length > 0 ? client.directories[client.directories.length - 1].path : '/'
    }

    HistoryModel {
        id: historyModel
        path: input.path
    }

    Client {
        id: client
        connectionName: 'carpet'
        onRespond: {
//            console.debug(message.action)
            switch (message.action) {
            case 'contents':
                client.contents = message.data
                break
            case 'diff':
                client.contents = message.data
                break
            }
        }

        property var directories: []
        property var file
        property var contents: {data: ''}

        onFileChanged: {
            if (!input.ready) return
            if (input.backup > 0)
                client.request({action: 'diff', path: file.path, backup: input.backup}, client)
            else
                client.request({action: 'contents', path: file.path, backup: ''}, client)
        }
    }


    QVBoxLayout {
        _id: 'main'
        MenuBar {}

        QWidget {
            size: Qt.size(10, 30)
            sizePolicy: Qt.size(1, 0)
            Ul {
                _id: 'directories'
                Li {
                    A {
                        href: http.path
                        Icon {
                            src: MimeType.iconNameForMimeType('inode/directory')
                            title: 'inode/directory'
                        }
                    }
                    Text { text: '/' }
                }
                Repeater {
                    model: pathModel
                    Component {
                        Li {
                            enabled: model.mime === 'inode/directory'
                            A {
                                href: '%1?path=%2/'.arg(http.path).arg(model.path)
                                title: model.mime
                                Text { text: model.name }
                            }
                            Text { text: '/' }
                        }
                    }
                }
            }
        }
        QHSplitter {
            QListView {
                _id: 'entries'
                size: Qt.size(200, 0)
                sizePolicy: Qt.size(0, 1)
                Repeater {
                    id: entries
                    model: entryModel
                    property alias path: input.path
                    Component {
                        Li {
                            _class: entries.path === (model.mime === 'inode/directory' ? model.path + '/' : model.path)? 'selected' : ''
                            A {
                                _class: model.removed ? 'removed' : ''
                                href: (model.mime === 'inode/directory' ? '%1?path=%2/' : '%1?path=%2').arg(http.path).arg(model.path)
                                Icon {
                                    src: MimeType.iconNameForMimeType(model.mime)
                                    title: model.mime
                                }
                                Text { text: model.name }
                            }
                        }
                    }
                }
            }

            QTextEdit {
                _id: 'contents'
                sizePolicy: Qt.size(1, 1)
                sizeHint: Qt.size(1, 2)
                property string stringData: client.contents && client.contents.data ? client.contents.data : ''
                text: stringData.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;')
            }

            QListView {
                _id: 'histories'
                size: Qt.size(200, 0)
                sizePolicy: Qt.size(0, 1)
                Li {
                    enabled: !client.file.removed
                    _class: input.backup < 0 ? 'selected' : ''
                    A {
                        href: (client.file.mime === 'inode/directory' ? '%1?path=%2/' : '%1?path=%2').arg(http.path).arg(client.file.path)
                        Icon {
                            src: MimeType.iconNameForMimeType(client.file.mime)
                            title: client.file.mime
                        }
                        Text { text: 'Current' }
                    }
                }
                Repeater {
                    id: histories
                    model: historyModel
                    property alias backup: input.backup
                    Component {
                        Li {
                            _class: histories.backup === model.id ? 'selected' : ''
                            A {
                                href: (model.mime === 'inode/directory' ? '%1?path=%2/&backup=%3' : '%1?path=%2&backup=%3').arg(http.path).arg(model.path).arg(model.id)
                                Icon {
                                    src: MimeType.iconNameForMimeType(model.mime)
                                    title: model.mime
                                }
                                Text { text: Qt.formatDateTime(model.ctime, 'yyyy/MM/dd hh:mm') }
                            }
                        }
                    }
                }
            }
        }
        StatusBar {
            size: Qt.size(10, 30)
            sizePolicy: Qt.size(1, 0)
        }
    }

    QVBoxLayout {
        _id: "loading"
    }
}

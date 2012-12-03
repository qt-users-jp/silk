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
        property string action

        onSubmit: {
            input.ready = true

            switch (input.action) {
            case 'download':
                break
            case 'restore':
                break
            }

            historyModel.update()

            if (backup < 0) {
                if (historyModel.count() > 0)
                    backup = historyModel.get(0).id
                else
                    return
            }

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
        }
    }

    PathModel {
        id: pathModel
        path: input.path
        backup: input.backup
    }

    EntryModel {
        id: entryModel
        path: client.directories.length > 0 ? client.directories[client.directories.length - 1].path : '/'
        backup: input.backup
    }

    HistoryModel {
        id: historyModel
    }

    Client {
        id: client
        connectionName: 'carpet'
        onRespond: {
//            console.debug(message.action);
            switch (message.action) {
            case 'contents':
                client.contents = message.data
                break
            }
        }

        property var directories: []
        property var file
        property var contents: {data: ''}

        onFileChanged: {
            if (!input.ready) return;
            client.request({action: 'contents', backup: input.backup, path: file.path}, client);
        }
    }


    QStackedLayout {
        QVBoxLayout {
            _id: 'main'
            MenuBar {}

            QHSplitter {
                QListView {
                    _id: 'histories'
                    size: Qt.size(200, 0)
                    sizePolicy: Qt.size(0, 1)
                    Repeater {
                        id: histories
                        model: historyModel
                        property alias backup: input.backup
                        Component {
                            Li {
                                _class: input.backup === model.id ? 'selected' : ''
                                A {
                                    href: (model.mime === 'inode/directory' ? '%1?backup=%2' : '%1?backup=%2').arg(http.path).arg(model.id)
                                    Icon { src: '/icons/backuplist.png' }
                                    Text { text: Qt.formatDateTime(model.ctime, 'yyyy/MM/dd hh:mm') }
                                }
                            }
                        }
                    }
                }
                QVBoxLayout {
                    QWidget {
                        size: Qt.size(10, 30)
                        sizePolicy: Qt.size(1, 0)
                        Ul {
                            _id: 'directories'
                            Li {
                                enabled: input.backup > -1
                                A {
                                    href: '%1?backup=%2'.arg(http.path).arg(input.backup)
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
                                            href: '%1?backup=%2&path=%3/'.arg(http.path).arg(input.backup).arg(model.path)
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
                            Li {
                                enabled: input.backup > -1
                                _class: client.file && client.file.mime && client.file.mime === 'inode/directory' ? 'selected' : ''
                                A {
                                    href: '%1?backup=%2&path=%3/'.arg(http.path).arg(input.backup).arg(client.directories.length > 0 ? client.directories[client.directories.length - 1].path : '')
                                    Icon {
                                        src: MimeType.iconNameForMimeType(title)
                                        title: 'inode/directory'
                                    }
                                    Text { text: '.' }
                                }
                            }
                            Li {
                                enabled: client.directories.length == 1
                                A {
                                    href: '%1?backup=%2'.arg(http.path).arg(input.backup)
                                    Icon {
                                        src: MimeType.iconNameForMimeType(title)
                                        title: 'inode/directory'
                                    }
                                    Text { text: '..' }
                                }
                            }
                            Li {
                                enabled: client.directories.length > 1
                                A {
                                    href: '%1?backup=%2&path=%3/'.arg(http.path).arg(input.backup).arg(client.directories.length > 1 ? client.directories[client.directories.length - 2].path : '')
                                    Icon {
                                        src: MimeType.iconNameForMimeType(title)
                                        title: 'inode/directory'
                                    }
                                    Text { text: '..' }
                                }
                            }
                            Repeater {
                                id: entries
                                model: entryModel
                                property alias path: input.path
                                Component {
                                    Li {
                                        _class: entries.path === (model.mime === 'inode/directory' ? model.path + '/' : model.path)? 'selected' : ''
                                        A {
                                            href: (model.mime === 'inode/directory' ? '%1?backup=%2&path=%3/' : '%1?backup=%2&path=%3').arg(http.path).arg(input.backup).arg(model.path)
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

                        QVBoxLayout {
                            QHBoxLayout {
                                size: Qt.size(1, 30)
                                sizePolicy: Qt.size(1, 0)
                                QLabel {
                                    Text { text: 'Mime type:' }
                                    Icon {
                                        src: MimeType.iconNameForMimeType(client.file.mime)
                                        title: client.file.mime
                                    }
                                    Text { text: client.file.mime }
                                }
                                QLabel { text: 'Last modified: %1'.arg(Qt.formatDateTime(client.file.ctime, 'yyyy/MM/dd hh:mm')) }
                            }
                            QHBoxLayout {
                                size: Qt.size(1, 30)
                                sizePolicy: Qt.size(1, 0)
                                QLabel { text: 'Size: %1KB'.arg(client.file.size) }
                                QLabel { text: 'Owner: %1 / Group: %2 / Permissions: %3'.arg(client.file.ownerId).arg(client.file.groupId).arg(client.file.permissions.toString(16)) }
                            }
                            QHBoxLayout {
                                size: Qt.size(1, 30)
                                sizePolicy: Qt.size(1, 0)
                                QWidget {
                                    sizePolicy: Qt.size(1, 1)
                                }
                                QWidget {
                                    size: Qt.size(30, 30)
                                    sizePolicy: Qt.size(0, 0)
                                    A {
                                        href: '%1?action=download&backup=%2&path=%3'.arg(http.path).arg(input.backup).arg(client.file.mime === 'inode/directory' ? client.file.path + '/' : client.file.path)
                                        Icon {
                                            _id: 'download'
                                            src: '/icons/download.png'
                                            title: 'Download %1'.arg(client.file.name)
                                        }
                                    }
                                }
                                QWidget {
                                    size: Qt.size(30, 30)
                                    sizePolicy: Qt.size(0, 0)
                                    A {
                                        href: '%1?action=restore&backup=%2&path=%3'.arg(http.path).arg(input.backup).arg(client.file.mime === 'inode/directory' ? client.file.path + '/' : client.file.path)
                                        Icon {
                                            _id: 'restore'
                                            src: '/icons/restore.png'
                                            title: 'Restore %1'.arg(client.file.name)
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
                        }
                    }
                }
            }
            StatusBar {
                size: Qt.size(10, 30)
                sizePolicy: Qt.size(1, 0)
            }
        }
        QDialog {
            size: Qt.size(300, 200)
            _id: 'restore-confirm-dialog'
            action: http.path
            method: 'POST'
            _class: 'window'

            QVBoxLayout {
                QWidget {
                    size: Qt.size(100, 30)
                    Text { text: 'Are you sure?' }
                }
                QWidget {
                    size: Qt.size(100, 30)
                    Input { type: 'submit'; value: 'Remove' }
                    Input { type: 'hidden'; name: 'action'; value: 'restore' }
                    Input { _id: 'restore-id'; type: 'hidden'; name: 'id' }
                }
            }
        }
    }
}

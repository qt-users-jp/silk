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
import "./components/"
import "./settings/"

CarpetTemplate {
    id: html

    SilkConfig {
        id: defaultConfig
        property string from
        property string to
    }

    UserInput {
        id: input
        property bool ready: false
        property string action
        property string id
        property string pattern
        property string entire_directory
        onSubmit: {
            input.ready = true

            switch (action) {
            case 'add':
                client.request({action: 'addExcludePattern', pattern: input.pattern, entireDirectory: input.entire_directory}, client)
                break
            case 'remove':
                client.request({action: 'removeExcludePattern', id: input.id, pattern: input.pattern}, client)
                break
            case 'enable':
                client.request({action: 'setEnabled', enabled: true}, client)
                break
            case 'disable':
                client.request({action: 'setEnabled', enabled: false}, client)
                break
            case 'backup':
                client.request({action: 'backup', name: qsTr("Backup(manual)")}, client)
                break
            default:
                break;
            }

            client.request({action: 'enabled'}, client)
//            client.request({action: 'tree'}, client)
            client.request({action: 'excludePatterns'}, client)
        }
    }

    Client {
        id: client
        connectionName: 'carpet'
        onRespond: {
//            console.debug(message.action);
            switch (message.action) {
            case 'enabled':
                if (message.enabled)
                    enabled._class = 'selected'
                else
                    disabled._class = 'selected'
                break;
            case 'tree':
                dirTree.model = message.data;
                break;
            case 'excludePatterns':
                excludePatterns.model = message.data;
                break;
            }
        }
    }

    QStackedLayout {
        QVBoxLayout {
            _id: 'main'
            MenuBar {}

            QHSplitter {
                QVBoxLayout {
                    size: Qt.size(150, 1)
                    sizePolicy: Qt.size(0, 1)
                    QLabel { text: 'Carpet' }
                    QListView {
                        _id: 'carpet-enabled'
                        sizePolicy: Qt.size(1, 1)
                        Li {
                            id: enabled
                            A {
                                href: '%1?action=enable'.arg(http.path)
                                Img {
                                    src: '/icons/enabled.png'
                                    width: '22px'
                                    height: '22px'
                                }
                                Text { text: 'On' }
                            }
                        }
                        Li {
                            id: disabled
                            A {
                                href: '%1?action=disable'.arg(http.path)
                                Img {
                                    src: '/icons/disabled.png'
                                    width: '22px'
                                    height: '22px'
                                }
                                Text { text: 'Off' }
                            }
                        }
                        Li {
                            id: backupNow
                            A {
                                href: '%1?action=backup'.arg(http.path)
                                Img {
                                    src: '/icons/run.png'
                                    width: '22px'
                                    height: '22px'
                                }
                                Text { text: 'Backup Now' }
                            }
                        }
                    }
                    QLabel { text: 'Backup schedule:' }
                    QListView {
                        sizePolicy: Qt.size(1, 1)
                        sizeHint: Qt.size(1, 5)
                        Repeater {
                            model: [1, 2, 3, 4, 6, 12, 24]
                            Component {
                                Li {
                                    Text { text: qsTr('every %1 hour(s)').arg(model.modelData) }
                                }
                            }
                        }
                    }
                }
//                QVBoxLayout {
//                    sizeHint: Qt.size(2, 1)
//                    QLabel { text: 'Backup directories:' }

//                    DirectoryTreeView { id: dirTree }

//                }

                QVBoxLayout {
                    sizeHint: Qt.size(1, 1)
                    QLabel { text: 'Exclude these items from backups:' }
                    QListView {
                        sizePolicy: Qt.size(1, 1)
                        Li {
                            _id: 'add-exclude-pattern'
                            Img {
                                src: '/icons/list-add.png'
                                width: '22px'
                                height: '22px'
                            }
                            Text { text: 'Add a new pattern' }
                        }
                        Repeater {
                            id: excludePatterns
                            Component {
                                Li {
//                                    A {
//                                        href: '%1?action=remove&id=%2&pattern=%3'.arg(http.path).arg(model.id).arg(model.pattern)
                                        Img {
                                            src: '/icons/list-remove.png'
                                            width: '22px'
                                            height: '22px'
                                            property string data_pattern_id: model.id
                                            property string data_pattern: model.pattern
                                            property string data_entire_directory: model.entireDirectoryIfExists
                                            _class: 'remove-exclude-pattern'
                                        }
//                                    }
                                    Img {
                                        src: model.entireDirectoryIfExists ? '/icons/folder.png' : '/icons/file.png'
                                        width: '22px'
                                        height: '22px'
                                    }
                                    Text { text: '/'; enabled: model.entireDirectoryIfExists }
                                    Text { text: model.pattern }
                                }
                            }
                        }
                    }
                }
                QVBoxLayout {
                    sizeHint: Qt.size(1, 1)
                    QLabel { text: 'Backup from' }
                    QListView {
                        sizePolicy: Qt.size(1, 1)
                    }
                }
            }
            StatusBar {
                size: Qt.size(10, 30)
                sizePolicy: Qt.size(1, 0)
            }
        }

        QDialog {
            size: Qt.size(300, 400)
            _id: 'add-exclude-pattern-dialog'
            action: http.path
            method: 'POST'
            _class: 'window'

            QVBoxLayout {
                QWidget {
                    size: Qt.size(100, 30)
                    Text { text: 'Pattern:' }
                    Input { _id: 'pattern'; type: 'text'; name: 'pattern'; placeholder: '*.o, Makefile* ...' }
                }
                QListView {
                    size: Qt.size(100, 30)
                    sizePolicy: Qt.size(1, 1)

                }

                QWidget {
                    size: Qt.size(100, 60)
                    Input { _id: 'entire-directory'; type: 'checkbox'; name: 'entire-directory' }
                    Label { _for: 'entire-directory'; text: 'Exclude entire directory containing matched files' }
                }
                QWidget {
                    size: Qt.size(100, 30)
                    Input { type: 'submit'; value: 'Add' }
                    Input { type: 'hidden'; name: 'action'; value: 'add' }
                }
            }
        }

        QDialog {
            size: Qt.size(300, 200)
            _id: 'remove-exclude-pattern-dialog'
            action: http.path
            method: 'POST'
            _class: 'window'

            QVBoxLayout {
                QWidget {
                    size: Qt.size(100, 30)
                    Text { text: 'Pattern:' }
                    Span { _id: 'remove-pattern' }
                }
                QWidget {
                    size: Qt.size(100, 30)
                    Input { type: 'submit'; value: 'Remove' }
                    Input { type: 'hidden'; name: 'action'; value: 'remove' }
                    Input { _id: 'remove-pattern-id'; type: 'hidden'; name: 'id' }
                }
            }
        }
    }
}

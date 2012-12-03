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
import Silk.Utils 1.0
import Carpet 1.0

Server {
    id: root
    connectionName: 'carpet'

    Backup { id: backup }

    onRequest: {
//        console.debug('+ Server::onRequest', message.action)
        switch (message.action) {
        case 'status':
            message.data = backup.status
            sender.respond(message)
            break
        case 'contents':
            message.data = backup.contents(message.path, message.backup)
            sender.respond(message)
            break
        case 'diff':
            message.data = backup.diff(message.path, message.backup)
            sender.respond(message)
            break
        case 'tree':
            message.data = backup.tree()
            sender.respond(message)
            break
        case 'excludePatterns':
            message.data = backup.excludePatterns()
            sender.respond(message)
            break
        case 'addExcludePattern':
            message.data = backup.addExcludePattern(message.pattern, message.entireDirectory)
            sender.respond(message)
            break
        case 'removeExcludePattern':
            message.data = backup.removeExcludePattern(message.id, message.pattern)
            sender.respond(message)
            break
        case 'enabled':
            message.enabled = backup.enabled
            sender.respond(message)
            break
        case 'setEnabled':
            backup.enabled = message.enabled
            sender.respond(message)
            break
        case 'backup':
            backup.backup(message.name)
            sender.respond(message)
            break
        case 'jobs':
            message.data = backup.jobList()
            sender.respond(message)
            break
        }
//        console.debug('- Server::onRequest', message.action)
    }

    Timer {
        repeat: false
        onTriggered: backupTimer.start()
        Component.onCompleted: {
            var date = new Date()
            var min = date.getMinutes() % 30;
            interval = ((30 - min) * 60 - date.getSeconds()) * 1000;
            start()
        }
    }

    Timer {
        id: backupTimer
        repeat: true
        running: false
        interval: 30 * 60 * 1000
        triggeredOnStart: true
        onTriggered: if (backup.enabled) backup.backup(qsTr("Backup(scheduled)"))
    }

//    Component.onCompleted: backup.backup()
}

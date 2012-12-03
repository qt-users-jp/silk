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

#include "historymodel.h"
#include "carpetfileinfo.h"
#include "carpetdb.h"
#include <silkconfig.h>

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QMutexLocker>
#include <QtCore/QStringList>
#include <QtSql/QSqlQuery>

HistoryModel::HistoryModel(QObject *parent)
    : CarpetAbstractModel(parent)
    , m_backup(-1)
{
}

void HistoryModel::update()
{
    qDebug() << Q_FUNC_INFO << __LINE__ << m_path << m_backup;
    QMutexLocker locker(&CarpetDB::mutex());
    qDebug() << Q_FUNC_INFO << __LINE__;

    QSqlDatabase db = CarpetDB::database();

    if (m_path.isEmpty()) {
        QSqlQuery query(db);
        query.prepare("SELECT Backup.backupID"
                      ", Backup.backupDate"
                      " FROM Backup"
                      " ORDER BY Backup.backupID DESC"
                      );
        if (CarpetDB::exec(query)) {
            while (query.next()) {
                CarpetFileInfo fi;
                fi.id = query.value(0).toInt();
                fi.cTime = query.value(1).toDateTime();
                append(fi);
            }
        }
    } else {
        int fileId = -1;
        {
            QSqlQuery query(db);
            query.prepare("SELECT fileId FROM File WHERE directoryID IS NULL");
            if (!CarpetDB::exec(query)) return;
            if (!query.first()) return;
            fileId = query.value(0).toInt();
        }
        foreach (const QString &item, m_path.split("/", QString::SkipEmptyParts)) {
            QSqlQuery query(db);
            query.prepare("SELECT fileId FROM File WHERE directoryID = ? AND fileName = ?");
            query.addBindValue(fileId);
            query.addBindValue(item);
            if (!CarpetDB::exec(query)) return;
            if (!query.first()) return;
            fileId = query.value(0).toInt();
        }

        QSqlQuery query(db);
        query.prepare("SELECT Backup.backupID"
                      ", Backup.backupDate"
                      ", MimeType.mimeTypeName"
                      " FROM Backup"
                      " INNER JOIN FileTree ON FileTree.backupID = Backup.backupID"
                      " INNER JOIN BackupData ON BackupData.fileTreeID = FileTree.fileTreeID"
                      " INNER JOIN MimeType ON MimeType.mimeTypeID = BackupData.mimeTypeID"
                      " WHERE FileTree.fileID = ?"
                      " ORDER BY Backup.backupID DESC");
        query.addBindValue(fileId);
        if (CarpetDB::exec(query)) {
            while (query.next()) {
                CarpetFileInfo fi;
                fi.id = query.value(0).toInt();
                fi.path = m_path;
                fi.cTime = query.value(1).toDateTime();
                fi.mimeType = query.value(2).toString();
                append(fi);
            }
        }
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

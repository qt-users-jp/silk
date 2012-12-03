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

#include "pathmodel.h"

#include "carpetfileinfo.h"
#include "carpetdb.h"
#include <silkconfig.h>

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QMimeDatabase>
#include <QtCore/QMutexLocker>
#include <QtCore/QStringList>
#include <QtSql/QSqlQuery>

PathModel::PathModel(QObject *parent)
    : CarpetAbstractModel(parent)
    , m_backup(-1)
{
}

void PathModel::update()
{
    qDebug() << Q_FUNC_INFO << __LINE__ << m_path << m_backup;
    QMutexLocker locker(&CarpetDB::mutex());
    qDebug() << Q_FUNC_INFO << __LINE__;

    QMimeDatabase mimeDatabase;
    QSqlDatabase db = CarpetDB::database();

    QStringList items = m_path.split("/", QString::SkipEmptyParts);
    QString fullPath = "";
    int fileId = -1;

    {
        QSqlQuery query(db);
        query.prepare("SELECT fileId FROM File WHERE directoryID IS NULL");
        if (!CarpetDB::exec(query)) return;
        if (!query.first()) return;
        fileId = query.value(0).toInt();
    }

    foreach (const QString &item, items) {
        fullPath += "/" + item;
        QFileInfo fileInfo(SilkConfig::value("carpet.from").toString() + fullPath);

        {
            QSqlQuery query(db);
            query.prepare("SELECT fileID"
                          " FROM File"
                          " WHERE fileName = ?"
                          " AND directoryID = ?");
            query.addBindValue(item);
            query.addBindValue(fileId);
            if (!CarpetDB::exec(query)) return;
            if (query.first()) {
                fileId = query.value(0).toInt();
            } else if (fileInfo.exists() ){
                QSqlQuery query2(db);
                query2.prepare("INSERT INTO File(directoryId, fileName) VALUES(?, ?)");
                query2.addBindValue(fileId);
                query2.addBindValue(item);
                if (!CarpetDB::exec(query2)) return;
                fileId = query2.lastInsertId().toInt();
            } else {
                qWarning() << Q_FUNC_INFO << __LINE__ << item << "is not found under" << fileId;
            }
        }

        CarpetFileInfo fi;
        fi.name = item;
        fi.path = fullPath;

        if (fileInfo.exists() && m_backup < 0) {
            if (fileInfo.isSymLink()) {
                fi.mimeType = "inode/symlink";
            } else if (fileInfo.isDir()) {
                fi.mimeType = "inode/directory";
            } else if (fileInfo.isFile()) {
                QMimeType mimeType = mimeDatabase.mimeTypeForFile(fileInfo);
                fi.mimeType = mimeType.name();
            } else {
                QMimeType mimeType = mimeDatabase.mimeTypeForFile(fileInfo);
                fi.mimeType = mimeType.name();
            }
            fi.size = fileInfo.size();
            fi.permissions = static_cast<uint>(fileInfo.permissions());
            fi.ownerId = fileInfo.ownerId();
            fi.groupId = fileInfo.groupId();
            fi.cTime = fileInfo.created();
            fi.mTime = fileInfo.lastModified();
            fi.aTime = fileInfo.lastRead();
            fi.removed = false;
            append(fi);
        } else {
            QString sql = "SELECT BackupData.size"
                    ", MimeType.mimeTypeName"
                    ", BackupData.permissions"
                    ", BackupData.uID"
                    ", BackupData.gID"
                    ", BackupData.cTime"
                    ", BackupData.mTime"
                    ", BackupData.aTime"
                    " FROM BackupData"
                    " INNER JOIN FileTree ON FileTree.fileTreeID = BackupData.fileTreeID"
                    " INNER JOIN MimeType ON BackupData.mimeTypeID = MimeType.mimeTypeID"
                    " WHERE FileTree.fileID = ?";
            if (m_backup > 0) sql += " AND FileTree.backupID <= ?";
            sql += " ORDER BY FileTree.fileTreeID DESC"
                    " LIMIT 1";
            QSqlQuery query(sql, db);
            query.addBindValue(fileId);
            if (m_backup > 0) query.addBindValue(m_backup);
            if (CarpetDB::exec(query)) {
                if (query.first()) {
                    fi.size = query.value(0).toInt();
                    fi.mimeType = query.value(1).toString();
                    fi.permissions = query.value(2).toUInt();
                    fi.ownerId = query.value(3).toUInt();
                    fi.groupId = query.value(4).toUInt();
                    fi.cTime = query.value(5).toDateTime();
                    fi.mTime = query.value(6).toDateTime();
                    fi.aTime = query.value(7).toDateTime();
                    fi.removed = true;
                    append(fi);
                } else {
                    qWarning() << Q_FUNC_INFO << __LINE__ << fileId << "not found.";
                }
            }
        }
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

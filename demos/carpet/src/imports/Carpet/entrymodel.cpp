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

#include "entrymodel.h"
#include "carpetfileinfo.h"
#include "carpetdb.h"
#include <silkconfig.h>

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QMimeDatabase>
#include <QtCore/QMutexLocker>
#include <QtCore/QStringList>
#include <QtSql/QSqlQuery>

EntryModel::EntryModel(QObject *parent)
    : CarpetAbstractModel(parent)
    , m_backup(-1)
{
}

void EntryModel::update()
{
    qDebug() << Q_FUNC_INFO << __LINE__ << m_path << m_backup;
    QMutexLocker locker(&CarpetDB::mutex());
    qDebug() << Q_FUNC_INFO << __LINE__;

    QMimeDatabase mimeDatabase;
    QSqlDatabase db = CarpetDB::database();
    QString from = SilkConfig::value("carpet.from").toString();
    QDir dir(from + m_path);
    QStringList added;

    if (m_backup < 0) {
        foreach (const QFileInfo &fileInfo, dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::Readable | QDir::Hidden, QDir::Name)) {
            CarpetFileInfo fi;
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
            fi.name = fileInfo.fileName();
            QString p = QDir::cleanPath(dir.absoluteFilePath(fileInfo.fileName()).mid(from.length()));
            if (p == "/") p = "";
            fi.path = p;
            fi.size = fileInfo.size();
            fi.permissions = static_cast<uint>(fileInfo.permissions());
            fi.ownerId = fileInfo.ownerId();
            fi.groupId = fileInfo.groupId();
            fi.cTime = fileInfo.created();
            fi.mTime = fileInfo.lastModified();
            fi.aTime = fileInfo.lastRead();
            fi.removed = false;
            added.append(fileInfo.fileName());
    //        qDebug() << Q_FUNC_INFO << __LINE__ << p;
            if (!p.contains("/.."))
                append(fi);
        }
    }

    int directoryId = -1;
    {
        QSqlQuery query(db);
        query.prepare("SELECT fileId FROM File WHERE directoryID IS NULL");
        if (!CarpetDB::exec(query)) return;
        if (!query.first()) return;
        directoryId = query.value(0).toInt();
    }
    foreach (const QString &item, m_path.split("/", QString::SkipEmptyParts)) {
        QSqlQuery query(db);
        query.prepare("SELECT fileId FROM File WHERE directoryID = ? AND fileName = ?");
        query.addBindValue(directoryId);
        query.addBindValue(item);
        if (!CarpetDB::exec(query)) return;
        if (!query.first()) return;
        directoryId = query.value(0).toInt();
    }

    QString sql;

    QSqlQuery query(db);
    if (m_backup < 0) {
        sql = "SELECT File.fileID"
                ", File.fileName"
                ", max(FileTree.fileTreeID)"
                " FROM File"
                " INNER JOIN FileTree ON FileTree.fileID = File.fileID"
                " WHERE File.directoryID = ?"
                " GROUP BY File.fileID"
                ", File.fileName"
                " ORDER BY File.fileName";
        query.prepare(sql);
        query.addBindValue(directoryId);
    } else {
        sql = "SELECT File.fileID"
                ", File.fileName"
                ", FileTree.fileTreeID"
                " FROM File"
                " INNER JOIN FileTree ON FileTree.fileID = File.fileID"
                " WHERE File.directoryID = ?"
                " AND FileTree.backupID = ?"
                " ORDER BY File.fileName";
        query.prepare(sql);
        query.addBindValue(directoryId);
        query.addBindValue(m_backup);
    }
    if (CarpetDB::exec(query)) {
        while (query.next()) {
            int fileId = query.value(0).toInt();
            QString fileName = query.value(1).toString();
            int fileTreeId = query.value(2).toInt();
            if (added.contains(fileName)) continue;
            sql = "SELECT MimeType.mimeTypeName"
                    ", BackupData.size"
                    ", BackupData.cTime"
                    ", BackupData.mTime"
                    ", BackupData.aTime"
                    " FROM BackupData"
                    " INNER JOIN MimeType on MimeType.mimeTypeID = BackupData.mimeTypeID"
                    " INNER JOIN FileTree on FileTree.fileTreeID = BackupData.fileTreeID"
                    " WHERE FileTree.fileID = ?"
                    " AND BackupData.fileTreeID <= ?"
                    " ORDER BY FileTree.fileTreeID DESC"
                    " LIMIT 1";
            QSqlQuery query2(sql, db);
            query2.addBindValue(fileId);
            query2.addBindValue(fileTreeId);
            if (CarpetDB::exec(query2)) {
                if (query2.first()) {
                    CarpetFileInfo fi;
                    fi.mimeType = query2.value(0).toString();
                    fi.name = fileName;
                    fi.path = dir.absoluteFilePath(fileName).mid(from.length());
                    fi.size = query2.value(1).toInt();
                    fi.cTime = query2.value(2).toDateTime();
                    fi.mTime = query2.value(3).toDateTime();
                    fi.aTime = query2.value(4).toDateTime();
                    fi.removed = (m_backup < 0);
                    append(fi);
                } else {
                    qDebug() << Q_FUNC_INFO << __LINE__ << query2.lastQuery() << query2.boundValues();
                }
            }
        }
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

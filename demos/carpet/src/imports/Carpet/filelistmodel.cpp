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

#include "filelistmodel.h"
#include "carpetdb.h"
#include "carpetfileinfo.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QMutexLocker>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

FileListModel::FileListModel(QObject *parent)
    : CarpetAbstractModel(parent)
{
}

void FileListModel::readData(const QString &tableName)
{
    QMutexLocker locker(&CarpetDB::mutex());
    QSqlDatabase db = CarpetDB::database();
    QSqlQuery query(db);
    query.prepare(QString("SELECT fileID"
                          " FROM %1"
                          " ORDER BY sequence").arg(tableName)
                  );
    if (query.exec()) {
        while (query.next()) {
            addFile(query.value(0).toInt());
        }
    } else {
        qWarning() << Q_FUNC_INFO << __LINE__ << query.lastError();
        qWarning() << Q_FUNC_INFO << __LINE__ << query.lastQuery() << query.boundValues();
    }
}

void FileListModel::addFile(int fileId)
{
    CarpetFileInfo fileInfo;

    QSqlQuery query(CarpetDB::database());
    query.prepare("SELECT MimeType.mimeTypeName"
                  ", BackupData.size"
                  ", BackupData.hash"
                  ", BackupData.permissions"
                  ", BackupData.cTime"
                  ", BackupData.mTime"
                  ", BackupData.aTime"
                  " FROM FileTree"
                  " INNER JOIN BackupData ON BackupData.fileTreeID = FileTree.fileTreeID"
                  " INNER JOIN MimeType ON MimeType.mimeTypeID = BackupData.mimeTypeID"
                  " WHERE FileTree.fileID = ?"
                  " ORDER BY FileTree.fileTreeID DESC"
                  " LIMIT 1");
    query.addBindValue(fileId);
    if (query.exec()) {
        if (query.first()) {
            fileInfo.mimeType = query.value(0).toString();
            fileInfo.size = query.value(1).toInt();
            fileInfo.hash = query.value(2).toString();
            fileInfo.permissions = query.value(3).toUInt();
            fileInfo.cTime = query.value(4).toDateTime();
            fileInfo.mTime = query.value(5).toDateTime();
            fileInfo.aTime = query.value(6).toDateTime();
            fileInfo.id = fileId;

            int directoryId = fileId;
            QString fileName;
            QString path = "";
            forever {
                QSqlQuery query2(CarpetDB::database());
                query2.prepare("SELECT directoryID, fileName FROM File WHERE fileID = ?");
                query2.addBindValue(directoryId);
                if (query2.exec()) {
                    if (query2.first()) {
                        if (fileName.isEmpty()) {
                            fileName = query2.value(1).toString();
                        } else {
                            path = query2.value(1).toString() + "/" + path;
                        }
                        if (!query2.isNull(0)) {
                            directoryId = query2.value(0).toInt();
                        } else {
                            break;
                        }
                    } else {
                        qWarning() << Q_FUNC_INFO << __LINE__ << query2.lastQuery() << query2.boundValues();
                        break;
                    }
                } else {
                    qWarning() << Q_FUNC_INFO << __LINE__ << query2.lastError();
                    qWarning() << Q_FUNC_INFO << __LINE__ << query2.lastQuery() << query2.boundValues();
                    break;
                }
            }
            fileInfo.name = fileName;
            fileInfo.dir = path;
            append(fileInfo);
        } else {
            qWarning() << Q_FUNC_INFO << __LINE__ << query.lastQuery() << query.boundValues();
        }
    } else {
        qWarning() << Q_FUNC_INFO << __LINE__ << query.lastError();
        qWarning() << Q_FUNC_INFO << __LINE__ << query.lastQuery() << query.boundValues();
    }
}

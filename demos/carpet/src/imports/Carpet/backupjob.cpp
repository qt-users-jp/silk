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

#include "backupjob.h"
#include "abstractcarpetjobprivate_p.h"

#include <QtCore/QCryptographicHash>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QMimeDatabase>
#include <QtCore/QMutexLocker>
#include <QtCore/QRegExp>

uint qHash(const QRegExp &regexp) { return qHash(regexp.pattern()); }

struct BackupJobItem {
    QString path;
    int parentId;
};

class BackupJob::Private : public AbstractCarpetJob::Private
{
    Q_OBJECT
public:
    Private(BackupJob *parent);

public slots:
    virtual void start();

private slots:
    void backup();
    void cleanup();

private:
    int mimeTypeIdFor(const QFileInfo &fileInfo);
    int mimeTypeIdFor(const QString &mimeTypeName);
    int findFileId(const QString &fileName, int parentId);

private:
    BackupJob *q;
    QMimeDatabase mimeDatabase;

    int backupId;
    QDateTime since;
    QHash<QRegExp, bool> excludePatterns;

    QList<BackupJobItem> directories;
};

BackupJob::Private::Private(BackupJob *parent)
    : AbstractCarpetJob::Private(parent)
    , q(parent)
    , backupId(-1)
    , since(QDateTime::fromMSecsSinceEpoch(0))
{

}

void BackupJob::Private::start()
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    QMutexLocker lock(q->m_mutex);
    qDebug() << Q_FUNC_INFO << __LINE__;

    // start transaction
    if (!db.transaction()) {
        QMetaObject::invokeMethod(q, "error", Qt::QueuedConnection, Q_ARG(QString, tr("database is locked.")));
        return;
    }

    // check when last backup was
    {
        QSqlQuery query(db);
        query.prepare("SELECT Max(backupDate) FROM Backup");
        if (!exec(query)) {
            return;
        }
        if (query.first()) {
            since = query.value(0).toDateTime();
        }
    }

    // load exclude patterns
    {
        QSqlQuery query(db);
        query.prepare("SELECT pattern"
                      ", entireDirectoryIfExists"
                      " FROM ExcludePatterns");
        if (!exec(query)) {
            return;
        }
        while (query.next())
            excludePatterns.insert(QRegExp(query.value(0).toString(), Qt::CaseSensitive, QRegExp::Wildcard), query.value(1).toBool());
    }

    // add a backup
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO Backup (backupDate) VALUES(?)");
        query.addBindValue(QDateTime::currentDateTime());
        if (!exec(query)) {
            return;
        }
        backupId = query.lastInsertId().toInt();
    }

    // start backup
    BackupJobItem item;
    item.parentId = -1;
    item.path = backupFrom;
    directories.append(item);
    QMetaObject::invokeMethod(this, "backup", Qt::QueuedConnection);
}

void BackupJob::Private::backup()
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    QMutexLocker lock(q->m_mutex);
    BackupJobItem item = directories.takeFirst();
    qDebug() << Q_FUNC_INFO << __LINE__ << item.path << item.parentId;

    QDir dir(item.path);

    int directoryId = findFileId(dir.dirName(), item.parentId);
    // find directory id
    if (directoryId < 0) {
        error(tr("directory %1(%2) not found").arg(dir.dirName()).arg(item.parentId));
    }
    {
        //
    }

    QFileInfo fileInfo(dir.absolutePath());
    {
        int fileTreeId = -1;
        {
            QSqlQuery query(db);
            query.prepare("INSERT INTO FileTree (backupID, fileID) VALUES(?, ?)");
            query.addBindValue(backupId);
            query.addBindValue(directoryId);
            if (!exec(query)) {
                return;
            }
            fileTreeId = query.lastInsertId().toInt();
        }

        bool exists = false;
        {
            QSqlQuery query(db);
            query.prepare(
                        "SELECT BackupData.fileTreeID"
                        " FROM BackupData"
                        " INNER JOIN FileTree ON FileTree.fileTreeID = BackupData.fileTreeID"
                        " WHERE FileTree.fileID = ?"
                        );
            query.addBindValue(directoryId);
            if (!exec(query)) {
                return;
            }
            exists = query.first();
        }

        if (!exists || fileInfo.lastModified() > since) {
            bool toBeSaved = false;
            if (exists)
            {
                QSqlQuery query(db);
                query.prepare("SELECT BackupData.permissions"
                              ", BackupData.uID"
                              ", BackupData.gID"
                              " FROM BackupData"
                              " INNER JOIN FileTree ON FileTree.fileTreeID = BackupData.fileTreeID"
                              " WHERE FileTree.fileID = ?"
                              " ORDER BY FileTree.fileTreeID DESC"
                              " LIMIT 1"
                              );
                query.addBindValue(directoryId);
                if (!exec(query)) return;
                if (query.first()) {
                    if (static_cast<uint>(fileInfo.permissions()) != query.value(0).toInt()) toBeSaved = true;
                    if (fileInfo.ownerId() != query.value(1).toUInt()) toBeSaved = true;
                    if (fileInfo.groupId() != query.value(2).toUInt()) toBeSaved = true;
                }
            } else {
                toBeSaved = true;
            }

            if (toBeSaved) {
                QSqlQuery query("INSERT INTO BackupData (fileTreeID, mimeTypeID, permissions, uID, gID, cTime, mTime, aTime) VALUES(?, ?, ?, ?, ?, ?, ?, ?)", db);
                query.addBindValue(fileTreeId);
                query.addBindValue(mimeTypeIdFor("inode/directory"));
                query.addBindValue(static_cast<uint>(fileInfo.permissions()));
                query.addBindValue(fileInfo.ownerId());
                query.addBindValue(fileInfo.groupId());
                query.addBindValue(fileInfo.created());
                query.addBindValue(fileInfo.lastModified());
                query.addBindValue(fileInfo.lastRead());
                if (!exec(query)) return;
            }
        }
    }

    foreach (const QFileInfo &fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::Readable | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name)) {
        bool skip = false;
        foreach (const QRegExp &pattern, excludePatterns.keys()) {
            if (pattern.exactMatch(fileInfo.fileName())) {
                skip = true;
                break;
            }
        }
        if (skip) continue;


        if (fileInfo.isDir()) {
            if (fileInfo.absoluteFilePath() == backupTo) {
                skip = true;
            } else {
                foreach (const QRegExp &pattern, excludePatterns.keys()) {
                    if (excludePatterns.value(pattern)) {
                        if (QDir(fileInfo.absoluteFilePath()).entryList(QDir::AllEntries | QDir::Readable | QDir::NoDotAndDotDot | QDir::Hidden).indexOf(pattern) > -1) {
                            skip = true;
                        }
                    }
                }
            }
            if (!skip) {
                BackupJobItem item;
                item.parentId = directoryId;
                item.path = fileInfo.absoluteFilePath();
                directories.append(item);
            }
        } else if (fileInfo.isFile()) {

            int fileId = findFileId(fileInfo.fileName(), directoryId);
            if (fileId < 0) {
                error(tr("field id not found: %1(%2").arg(fileInfo.fileName().arg(directoryId)));
                return;
            }

            int fileTreeId = -1;
            {
                QSqlQuery query("INSERT INTO FileTree (backupID, fileID) VALUES(?, ?)", db);
                query.addBindValue(backupId);
                query.addBindValue(fileId);
                if (!exec(query)) return;
                fileTreeId = query.lastInsertId().toInt();
            }

            bool exists = false;
            {
                QSqlQuery query(db);
                query.prepare(
                            "SELECT BackupData.fileTreeID"
                            " FROM BackupData"
                            " INNER JOIN FileTree ON FileTree.fileTreeID = BackupData.fileTreeID"
                            " WHERE FileTree.fileID = ?"
                            );
                query.addBindValue(fileId);
                if (!exec(query)) return;
                exists = query.first();
            }

            if (!exists || fileInfo.lastModified() > since) {
                QFile in(fileInfo.absoluteFilePath());
                if (in.open(QFile::ReadOnly)) {
                    QByteArray data = in.readAll();
                    in.close();
                    int size = data.length();
                    data = qCompress(data, 9);
                    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

                    bool toBeSaved = false;
                    // check if file is changed
                    {
                        QSqlQuery query(db);
                        query.prepare("SELECT BackupData.hash"
                                      ", BackupData.permissions"
                                      ", BackupData.uID"
                                      ", BackupData.gID"
                                      " FROM BackupData"
                                      " INNER JOIN FileTree ON FileTree.fileTreeID = BackupData.fileTreeID"
                                      " WHERE FileTree.fileID = ?"
                                      " ORDER BY FileTree.fileTreeID DESC"
                                      " LIMIT 1");
                        query.addBindValue(directoryId);
                        if (!exec(query)) return;
                        if (query.first()) {
                            if (query.value(0).toString() != hash) toBeSaved = true;
                            if (query.value(1).toUInt() != static_cast<uint>(fileInfo.permissions())) toBeSaved = true;
                            if (query.value(2).toUInt() != fileInfo.ownerId()) toBeSaved = true;
                            if (query.value(3).toUInt() != fileInfo.groupId()) toBeSaved = true;
                        }
                    }

                    if (toBeSaved)
                    {
                        QSqlQuery query(db);
                        query.prepare("INSERT INTO BackupData (fileTreeID, size, mimeTypeID, hash, permissions, uID, gID, cTime, mTime, aTime) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                        query.addBindValue(fileTreeId);
                        query.addBindValue(size);
                        query.addBindValue(mimeTypeIdFor(fileInfo));
                        query.addBindValue(hash);
                        query.addBindValue(static_cast<uint>(fileInfo.permissions()));
                        query.addBindValue(fileInfo.ownerId());
                        query.addBindValue(fileInfo.groupId());
                        query.addBindValue(fileInfo.created());
                        query.addBindValue(fileInfo.lastModified());
                        query.addBindValue(fileInfo.lastRead());
                        if (!exec(query)) return;

                        QFile out(QString("%1/%2").arg(backupTo).arg(QString(hash.insert(2, "/"))));
                        if (!dir.mkpath(QString("%1/%2").arg(backupTo).arg(QString(hash.left(2))))) {
                            error(tr("%1/%2 could not be made.").arg(backupTo).arg(QString(hash.left(2))));
                            return;
                        }
                        if (!out.exists()) {
                            if (!out.open(QFile::WriteOnly)) {
                                qWarning() << Q_FUNC_INFO << __LINE__ << out.fileName() << out.errorString();
                                error(tr("%1 could not be open").arg(out.fileName()));
                                return;
                            }
                            out.write(data);
                            out.close();
                        }
                    }

                } else {
                    // TODO can be removed
                    qWarning() << Q_FUNC_INFO << __LINE__ << fileInfo.absoluteFilePath() << in.errorString();
                    QSqlQuery query("DELETE FROM FileTree WHERE fileTreeID = ?", db);
                    query.addBindValue(fileTreeId);
                    if (!exec(query)) return;
                }
            }
        }
    }

    if (directories.isEmpty()) {
        QMetaObject::invokeMethod(this, "cleanup", Qt::QueuedConnection);
    } else {
        QMetaObject::invokeMethod(this, "backup", Qt::QueuedConnection);
    }
}

int BackupJob::Private::findFileId(const QString &fileName, int parentId)
{
    int ret = -1;
    QSqlQuery query(db);
    if (parentId < 0) {
        query.prepare("SELECT fileID"
                      " FROM File"
                      " WHERE directoryID IS NULL");
    } else {
        query.prepare("SELECT fileID"
                      " FROM File"
                      " WHERE fileName = ?"
                      " AND directoryID = ?");
        query.addBindValue(fileName);
        query.addBindValue(parentId);
    }

    if (!exec(query)) return ret;
    if (query.first()) {
        ret = query.value(0).toInt();
    } else {
        QSqlQuery query2(db);
        if (parentId < 0) {
            query2.prepare("INSERT INTO File(fileName) VALUES(?)");
        } else {
            query2.prepare("INSERT INTO File(directoryId, fileName) VALUES(?, ?)");
            query2.addBindValue(parentId);
        }
        query2.addBindValue(fileName);
        if (!exec(query2)) return ret;
        ret = query2.lastInsertId().toInt();
    }

    return ret;
}

void BackupJob::Private::cleanup()
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    QMutexLocker lock(q->m_mutex);
    qDebug() << Q_FUNC_INFO << __LINE__;
    {
        QSqlQuery query(db);
        query.prepare("SELECT count(FileTree.fileID)"
                      " FROM FileTree"
                      " INNER JOIN BackupData ON BackupData.fileTreeID = FileTree.fileTreeID"
                      " WHERE FileTree.BackupID = ?");
        query.addBindValue(backupId);
        if (exec(query)) {
            if (query.first()) {
                if (query.value(0).toInt() == 0) {
                    // rollback or remove all filetree?
                    db.rollback();
                    QMetaObject::invokeMethod(q, "finished", Qt::QueuedConnection);
                    return;
                }
            } else {
                qDebug() << Q_FUNC_INFO << __LINE__;
            }
        }
    }
    if (db.commit()) {
        QMetaObject::invokeMethod(q, "finished", Qt::QueuedConnection);
    } else {
        error(tr("commit failed"));
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

int BackupJob::Private::mimeTypeIdFor(const QFileInfo &fileInfo)
{
    QString mimeTypeName;
    if (fileInfo.isSymLink()) {
        mimeTypeName = QLatin1String("inode/symlink");
    } else if (fileInfo.isDir()) {
        mimeTypeName = QLatin1String("inode/directory");
    } else if (fileInfo.isFile()) {
        QMimeType mimeType = mimeDatabase.mimeTypeForFile(fileInfo);
        mimeTypeName = mimeType.name();
    } else {
        QMimeType mimeType = mimeDatabase.mimeTypeForFile(fileInfo);
        mimeTypeName = mimeType.name();
    }

    return mimeTypeIdFor(mimeTypeName);
}

int BackupJob::Private::mimeTypeIdFor(const QString &mimeTypeName)
{
    static QMap<QString, int> cache;
    if (!cache.contains(mimeTypeName)) {
        QSqlQuery query("SELECT mimeTypeID FROM MimeType WHERE mimeTypeName = ?", db);
        query.addBindValue(mimeTypeName);
        exec(query);
        if (query.first()) {
            cache.insert(mimeTypeName, query.value(0).toInt());
        } else {
            QSqlQuery query2("INSERT INTO MimeType (mimeTypeName) VALUES(?)", db);
            query2.addBindValue(mimeTypeName);
            if (exec(query2)) {
                cache.insert(mimeTypeName, query2.lastInsertId().toInt());
            } else {
                qWarning() << Q_FUNC_INFO << __LINE__;
            }
        }
    }
    return cache.value(mimeTypeName);
}

BackupJob::BackupJob(QObject *parent)
    : AbstractCarpetJob(parent)
    , d(new Private(this))
{
}

void BackupJob::start()
{
    QMetaObject::invokeMethod(d, "start", Qt::QueuedConnection);
}

#include "backupjob.moc"

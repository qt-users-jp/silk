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

#include "backup.h"
#include "carpetdb.h"

#include <sys/types.h>
#include <utime.h>

#include <QtCore/QCryptographicHash>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QMimeDatabase>
#include <QtCore/QMutexLocker>
#include <QtCore/QProcess>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <silkconfig.h>

#include "backupjob.h"
#include "updatestatisticsjob.h"
#include "addexcludepatternjob.h"
#include "removeexcludepatternjob.h"

class Backup::Private : public QObject
{
    Q_OBJECT
public:
    Private(Backup *parent);
    QVariantMap contents(const QString &path, const QString &backup) const;
    QVariantMap diff(const QString &path, const QString &backup) const;
    QVariantList tree(const QString &path = QString()) const;
    QVariantList excludePatterns() const;

    bool exec(const QString &sql) const;
    bool exec(QSqlQuery &query) const;

public slots:
    void backup(const QString &name);
    bool restore(int fileTreeId, const QString &to, const QString &path = QString());
    void addExcludePattern(const QString &pattern, bool entireDirectory);
    void removeExcludePattern(const QString &id, const QString &pattern);

signals:
    void done(bool ok, const QString &errorMessage);

private slots:
    void finished();
    void error(const QString &errorMessage);
    void slotDone(bool ok, const QString &errorMessage) {
        qDebug() << Q_FUNC_INFO << __LINE__ << ok << errorMessage;
    }

private:
    void setupDatabase();
    bool createTables();

    QString getFullPath(int fileId) const;
    QVariantMap fileInfo(int fileId) const;

private:
    Backup *q;
    QMimeDatabase mimeDatabase;
public:
    Status status;
    QList<AbstractCarpetJob *> jobQueue;
    QSqlDatabase db;
    QString from;
    QString to;
};

Backup::Private::Private(Backup *parent)
    : q(parent)
    , status(Backup::Waiting)
    , from(SilkConfig::value("carpet.from").toString())
    , to(SilkConfig::value("carpet.to").toString())
{
    connect(this, SIGNAL(done(bool,QString)), this, SLOT(slotDone(bool,QString)));
    setupDatabase();
}

void Backup::Private::setupDatabase()
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    QMutexLocker locker(&CarpetDB::mutex());
    qDebug() << Q_FUNC_INFO << __LINE__;
    db = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"), QLatin1String("carpet"));
    db.setDatabaseName(QString("%1/carpet.sqlite").arg(to));
    if (db.open()) {
        if (db.transaction()) {
            if (createTables()) {
                if (!db.commit()) db.rollback();
            } else {
                db.rollback();
            }
        }
    } else {
        qWarning() << Q_FUNC_INFO << __LINE__ << db.lastError().text();
    }
}

bool Backup::Private::createTables()
{
    {
        QString sql = "CREATE TABLE IF NOT EXISTS Settings ("
                "key TEXT UNIQUE NOT NULL"
                ", value TEXT)";
        if (!exec(sql)) return false;
    }

    {
        QString sql = "CREATE TABLE IF NOT EXISTS Backup ("
                "backupID INTEGER PRIMARY KEY AUTOINCREMENT"
                ", backupDate TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                ", seconds INTEGER DEFAULT -1"
                ", annotation TEXT)";
        if (!exec(sql)) return false;
    }

    {
        QString sql = "CREATE TABLE IF NOT EXISTS File ("
                "fileID INTEGER PRIMARY KEY AUTOINCREMENT"
                ", directoryID INTEGER"
                ", fileName TEXT NOT NULL"
                ")";
        if (!exec(sql)) return false;
    }

    {
        QString sql = "CREATE INDEX IF NOT EXISTS File_directoryID ON File (directoryID)";
        if (!exec(sql)) return false;
    }

    {
        QString sql = "CREATE TABLE IF NOT EXISTS FileTree ("
                "fileTreeID INTEGER PRIMARY KEY AUTOINCREMENT"
                ", backupID INTEGER NOT NULL"
                ", fileID INTEGER NOT NULL)";
        if (!exec(sql)) return false;
    }

    {
        QString sql = "CREATE INDEX IF NOT EXISTS FileTree_backupID ON FileTree (backupID)";
        if (!exec(sql)) {
            return false;
        }
    }

    {
        QString sql = "CREATE INDEX IF NOT EXISTS FileTree_FileID ON FileTree (fileID)";
        if (!exec(sql)) {
            return false;
        }
    }

    {
        QString sql = "CREATE TABLE IF NOT EXISTS MimeType ("
                "mimeTypeID INTEGER PRIMARY KEY AUTOINCREMENT"
                ", mimeTypeName TEXT NOT NULL UNIQUE)";
        if (!exec(sql)) return false;
    }

    {
        QString sql = "CREATE TABLE IF NOT EXISTS BackupData ("
                "fileTreeID INTEGER NOT NULL UNIQUE"
                ", size INTEGER"
                ", mimeTypeID INTEGER NOT NULL"
                ", symlinkID INTEGER"
                ", hash TEXT"
                ", permissions INTEGER"
                ", uID INTEGER"
                ", gID INTEGER"
                ", cTime TIMESTAMP"
                ", mTime TIMESTAMP"
                ", aTime TIMESTAMP)";
        if (!exec(sql)) return false;
    }

    {
        QString sql = "CREATE INDEX IF NOT EXISTS BackupData_mimeTypeID ON BackupData (mimeTypeID)";
        if (!exec(sql)) return false;
    }

    {
        QString sql = "CREATE INDEX IF NOT EXISTS BackupData_fileTreeID ON BackupData (fileTreeID)";
        if (!exec(sql)) return false;
    }

    {
        QSqlQuery query(db);
        query.prepare("CREATE TABLE IF NOT EXISTS ExcludePatterns ("
                      "excludePatternID INTEGER PRIMARY KEY AUTOINCREMENT"
                      ", pattern TEXT NOT NULL UNIQUE"
                      ", entireDirectoryIfExists BOOLEAN NOT NULL DEFAULT 0"
                      ")");
        if (!exec(query)) return false;
    }

    {
        QSqlQuery query(db);
        query.prepare("CREATE TABLE IF NOT EXISTS RecentlyChanged ("
                      "sequence INTEGER NOT NULL UNIQUE"
                      ", fileID INTEGER NOT NULL UNIQUE"
                      ")");
        if (!exec(query)) return false;
    }

    {
        QSqlQuery query(db);
        query.prepare("CREATE TABLE IF NOT EXISTS FrequentlyChanged ("
                      "sequence INTEGER NOT NULL UNIQUE"
                      ", fileID INTEGER NOT NULL UNIQUE"
                      ")");
        if (!exec(query)) return false;
    }

    {
        QSqlQuery query(db);
        query.prepare("CREATE TABLE IF NOT EXISTS SizeConsumed ("
                      "sequence INTEGER NOT NULL UNIQUE"
                      ", fileID INTEGER NOT NULL UNIQUE"
                      ")");
        if (!exec(query)) return false;
    }
    return true;
}

void Backup::Private::backup(const QString &name)
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    {
        BackupJob *job = new BackupJob(this);
        job->setName(name);
        job->setMutex(&CarpetDB::mutex());
        connect(job, SIGNAL(finished()), this, SLOT(finished()));
        connect(job, SIGNAL(error(QString)), this, SLOT(error(QString)));
        if (jobQueue.isEmpty()) job->start();
        jobQueue.append(job);
    }
    {
        UpdateStatisticsJob *job = new UpdateStatisticsJob(this);
        job->setName(tr("Update statistics"));
        job->setMutex(&CarpetDB::mutex());
        connect(job, SIGNAL(finished()), this, SLOT(finished()));
        connect(job, SIGNAL(error(QString)), this, SLOT(error(QString)));
        if (jobQueue.isEmpty()) job->start();
        jobQueue.append(job);
    }
}

void Backup::Private::finished()
{
    AbstractCarpetJob *job = jobQueue.takeFirst();
    qDebug() << Q_FUNC_INFO << __LINE__ << job->name();
    job->deleteLater();
    if (!jobQueue.isEmpty()) {
        qDebug() << Q_FUNC_INFO << __LINE__ << jobQueue.first()->name();
        jobQueue.first()->start();
    }
}

void Backup::Private::error(const QString &errorMessage)
{
    AbstractCarpetJob *job = qobject_cast<AbstractCarpetJob *>(sender());
    qDebug() << Q_FUNC_INFO << __LINE__ << job->name() << errorMessage;
    finished();
}

QString Backup::Private::getFullPath(int fileId) const
{
//    qDebug() << Q_FUNC_INFO << __LINE__ << fileId;
    QString ret;
    QSqlQuery query("SELECT directoryID, fileName FROM File WHERE fileID = ?", db);
    query.addBindValue(fileId);
    if (!exec(query)) return ret;
    if (!query.first()) return ret;
    if (query.value(0).isNull())
        ret = query.value(1).toString();
    else
        ret = getFullPath(query.value(0).toInt()) + "/" + query.value(1).toString();
//    qDebug() << Q_FUNC_INFO << __LINE__ << ret;
    return ret;
}

bool Backup::Private::restore(int fileTreeId, const QString &to, const QString &path)
{
//    qDebug() << Q_FUNC_INFO << __LINE__ << fileTreeId << to << path;
    int backupId = -1;
    int fileId = -1;
    QString fileName;
    QString fullPath;
    {
        QSqlQuery query("SELECT FileTree.backupID, FileTree.fileID, File.fileName FROM FileTree INNER JOIN File ON File.fileID = FileTree.fileID WHERE FileTree.fileTreeID = ?", db);
        query.addBindValue(fileTreeId);
        if (!exec(query)) return false;
        if (!query.first()) {
            qWarning() << Q_FUNC_INFO << __LINE__;
            return false;
        }
        backupId = query.value(0).toInt();
        fileId = query.value(1).toInt();
        fileName = query.value(2).toString();
        if (path.isNull()) {
            fullPath = to + "/" + getFullPath(fileId);
        } else {
            fullPath = to + path + fileName;
        }

//        qDebug() << Q_FUNC_INFO << __LINE__ << fullPath;
    }

    {
        QSqlQuery query("SELECT symlinkID, hash, permissions, uID, gID, cTime, mTime, aTime FROM BackupData WHERE fileTreeID = ?", db);
        query.addBindValue(fileTreeId);
        if (!exec(query)) return false;
        if (!query.first()) {
            qWarning() << Q_FUNC_INFO << __LINE__;
            return false;
        }
        int symlinkId = -1;
        if (!query.isNull(0))
            symlinkId = query.value(0).toInt();
        QString hash = query.value(1).toString();
        int permissions = query.value(2).toInt();
        uint ownerId = query.value(3).toUInt();
        uint groupId = query.value(4).toUInt();
        QDateTime ctime = query.value(5).toDateTime();
        QDateTime mtime = query.value(6).toDateTime();
        QDateTime atime = query.value(7).toDateTime();
//        qDebug() << Q_FUNC_INFO << __LINE__ << symlinkId << hash << permissions << ownerId << groupId;
        if (symlinkId > -1) {
            // Symlink
        } else if (hash.isEmpty()) {
            // Directory
            QDir dir(fullPath);
            if (!dir.exists()) {
                dir.mkpath(fullPath);
            }
            QFileInfo fileInfo(fullPath);
            if (fileInfo.permissions() != permissions) {
                QFile::setPermissions(fullPath, static_cast<QFile::Permissions>(permissions));
            }
            if (fileInfo.ownerId() != ownerId) {
                // TODO
            }
            if (fileInfo.groupId() != groupId) {
                // TODO
            }
            if (fileInfo.created() != ctime) {
                // TODO ?
            }
            if (fileInfo.lastModified() != mtime || fileInfo.lastRead() != atime) {
                utimbuf t;
                t.modtime = mtime.toTime_t();
                t.actime = atime.toTime_t();
                utime(fullPath.toUtf8().constData(), &t);
            }

            QStringList entryList = dir.entryList(QDir::AllEntries | QDir::Readable | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name);

            QSqlQuery query2("SELECT FileTree.fileTreeID, File.fileName FROM FileTree INNER JOIN File ON File.fileID = FileTree.fileID WHERE FileTree.backupID = ? AND File.directoryID = ?", db);
            query2.addBindValue(backupId);
            query2.addBindValue(fileId);
            if (!exec(query2)) return false;
            while (query2.next()) {
                entryList.removeAll(query2.value(1).toString());
                if (!restore(query2.value(0).toInt(), to, path + fileName + "/")) return false;
            }
            foreach (const QString &f, entryList) {
                QFileInfo fi(dir.absoluteFilePath(f));
                if (fi.isDir()) {
                    if (!dir.rmdir(f)) {
                        qWarning() << Q_FUNC_INFO << __LINE__ << dir.absoluteFilePath(f);
                    }
                } else if (fi.isFile()) {
                    if (!QFile::remove(dir.absoluteFilePath(f))) {
                        qWarning() << Q_FUNC_INFO << __LINE__ << dir.absoluteFilePath(f);
                    }
                } else {
                    qWarning() << Q_FUNC_INFO << __LINE__ << dir.absoluteFilePath(f);
                }
            }
        } else {
            QFileInfo fileInfo(fullPath);

            if (!fileInfo.exists() || fileInfo.lastModified() > mtime) {
                QFile in(this->to + "/" + hash.insert(2, "/"));
                if (!in.open(QFile::ReadOnly)) {
                    qWarning() << Q_FUNC_INFO << __LINE__ << in.fileName() << in.errorString();
                    return false;
                }

                QFile out(fullPath);
                out.setPermissions(static_cast<QFile::Permissions>(0x600));
                if (!out.open(QFile::WriteOnly)) {
                    qWarning() << Q_FUNC_INFO << __LINE__ << out.fileName() << out.errorString();
                    in.close();
                    return false;
                }

                out.write(qUncompress(in.readAll()));
                out.close();
                in.close();

                if (fileInfo.permissions() != permissions) {
                    QFile::setPermissions(fullPath, static_cast<QFile::Permissions>(permissions));
                }
                if (fileInfo.ownerId() != ownerId) {
                    // TODO
                }
                if (fileInfo.groupId() != groupId) {
                    // TODO
                }
                if (fileInfo.created() != ctime) {
                    // TODO ?
                }
                if (fileInfo.lastModified() != mtime || fileInfo.lastRead() != atime) {
                    utimbuf t;
                    t.modtime = mtime.toTime_t();
                    t.actime = atime.toTime_t();
                    utime(fullPath.toUtf8().constData(), &t);
                }
            }
        }
    }

    return true;
}

bool Backup::Private::exec(const QString &sql) const
{
    QSqlQuery query(db);
    query.prepare(sql);
    return exec(query);
}

bool Backup::Private::exec(QSqlQuery &query) const
{
//    QReadLocker locker(&lock);
    bool ret = query.exec();
    if (!ret) {
        qWarning() << Q_FUNC_INFO << __LINE__ << query.lastQuery() << query.boundValues() << query.lastError();
    }
    return ret;
}

QVariantMap Backup::Private::contents(const QString &path, const QString &backup) const
{
    qDebug() << Q_FUNC_INFO << __LINE__ << path << backup;
    QMutexLocker locker(&CarpetDB::mutex());
    qDebug() << Q_FUNC_INFO << __LINE__;
    QVariantMap ret;
    if (backup.isEmpty()) {
        QFile file(from + path);
        if (file.open(QFile::ReadOnly)) {
            ret.insert("data", file.readAll());
            file.close();
        }
    } else {
        int fileId = -1;
        {
            QSqlQuery query(db);
            query.prepare("SELECT fileId FROM File WHERE directoryID IS NULL");
            if (!exec(query)) return ret;
            if (!query.first()) return ret;
            fileId = query.value(0).toInt();
        }
        qDebug() << Q_FUNC_INFO << __LINE__ << fileId;
        foreach (const QString &item, path.split("/", QString::SkipEmptyParts)) {
            QSqlQuery query(db);
            query.prepare("SELECT fileId FROM File WHERE directoryID = ? AND fileName = ?");
            query.addBindValue(fileId);
            query.addBindValue(item);
            if (!exec(query)) return ret;
            if (!query.first()) return ret;
            fileId = query.value(0).toInt();
        }
        qDebug() << Q_FUNC_INFO << __LINE__ << fileId;

        {
            QSqlQuery query(db);
            query.prepare("SELECT BackupData.hash"
                          " FROM BackupData"
                          " INNER JOIN FileTree ON FileTree.fileTreeID = BackupData.fileTreeID"
                          " WHERE FileTree.backupID <= ?"
                          " AND FileTree.fileID = ?"
                          " ORDER BY FileTree.backupID DESC"
                          " LIMIT 1");
            query.addBindValue(backup.toInt());
            query.addBindValue(fileId);
            if (!exec(query)) return ret;
            if (!query.first()) return ret;
            QString hash = query.value(0).toString();
            qDebug() << Q_FUNC_INFO << __LINE__ << hash;
            QFile file(QString("%1/%2").arg(to).arg(hash.insert(2, "/")));
            if (file.open(QFile::ReadOnly)) {
                ret.insert("data", qUncompress(file.readAll()));
                file.close();
            } else {
                qDebug() << Q_FUNC_INFO << __LINE__ << to << hash;
            }
        }
    }
    qDebug() << Q_FUNC_INFO << __LINE__ << ret.keys();
    return ret;
}

QVariantMap Backup::Private::diff(const QString &path, const QString &backup) const
{
    qDebug() << Q_FUNC_INFO << __LINE__ << path << backup;
    QMutexLocker locker(&CarpetDB::mutex());
    qDebug() << Q_FUNC_INFO << __LINE__;
    QVariantMap ret;
    int fileId = -1;
    {
        QSqlQuery query(db);
        query.prepare("SELECT fileId FROM File WHERE directoryID IS NULL");
        if (!exec(query)) return ret;
        if (!query.first()) return ret;
        fileId = query.value(0).toInt();
    }
    foreach (const QString &item, path.split("/", QString::SkipEmptyParts)) {
        QSqlQuery query(db);
        query.prepare("SELECT fileId FROM File WHERE directoryID = ? AND fileName = ?");
        query.addBindValue(fileId);
        query.addBindValue(item);
        if (!exec(query)) return ret;
        if (!query.first()) return ret;
        fileId = query.value(0).toInt();
    }

    {
        QSqlQuery query(db);
        query.prepare("SELECT BackupData.hash"
                      " FROM BackupData"
                      " INNER JOIN FileTree ON FileTree.fileTreeID = BackupData.fileTreeID"
                      " WHERE FileTree.backupID = ?"
                      " AND FileTree.fileID = ?");
        query.addBindValue(backup.toInt());
        query.addBindValue(fileId);
        if (!exec(query)) return ret;
        if (!query.first()) return ret;
        QString hash = query.value(0).toString();
        QFile file(QString("%1/%2").arg(to).arg(hash.insert(2, "/")));
        if (file.open(QFile::ReadOnly)) {
            QByteArray old = qUncompress(file.readAll());
            file.close();
            if (QFile::exists(from + path)) {
                QProcess diff;
                QStringList args;
                args.append("-u");
                args.append("-d");
                args.append("-");
                args.append(from + path);
                diff.start("diff", args);
                diff.waitForStarted();
                diff.write(old);
                diff.closeWriteChannel();
                diff.waitForFinished();
                QByteArray data = diff.readAll();
                ret.insert("data", data);
            } else {
                ret.insert("data", old);
            }
        } else {
            qDebug() << Q_FUNC_INFO << __LINE__ << file.errorString();
        }
    }
    qDebug() << Q_FUNC_INFO << __LINE__ << ret.keys();
    return ret;
}

QVariantMap Backup::Private::fileInfo(int fileId) const
{
    QVariantMap ret;

    QSqlQuery query(db);
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
    if (exec(query)) {
        if (query.first()) {
            ret.insert("mime", query.value(0).toString());
            ret.insert("size", query.value(1).toInt());
            ret.insert("hash", query.value(2).toString());
            ret.insert("permissions", query.value(3).toUInt());
            ret.insert("ctime", query.value(4).toDateTime());
            ret.insert("mtime", query.value(5).toDateTime());
            ret.insert("atime", query.value(6).toDateTime());
            ret.insert("fileId", fileId);

            int directoryId = fileId;
            QString fileName;
            QString path = "";
            forever {
                QSqlQuery query2("SELECT directoryID, fileName FROM File WHERE fileID = ?", db);
                query2.addBindValue(directoryId);
                if (exec(query2)) {
                    if (query2.first()) {
                        directoryId = query2.value(0).toInt();
                        if (fileName.isEmpty()) {
                            fileName = query2.value(1).toString();
                        } else {
                            path = query2.value(1).toString() + "/" + path;
                        }
                    } else {
                        break;
                    }
                }
            }
            ret.insert("name", fileName);
            ret.insert("dir", path);
        }
    }
    return ret;
}

QVariantList Backup::Private::tree(const QString &path) const
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    QMutexLocker locker(&CarpetDB::mutex());
    qDebug() << Q_FUNC_INFO << __LINE__;
    QVariantList ret;

    QDir dir(path.isEmpty() ? from : path);
    foreach (const QString &item, dir.entryList(QDir::Dirs | QDir::Readable | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name)) {
        if (dir.absoluteFilePath(item) == to) continue;
        QVariantMap map;
        map.insert("name", item);
        map.insert("children", tree(dir.absoluteFilePath(item)));
        ret.append(map);
    }

    return ret;
}

QVariantList Backup::Private::excludePatterns() const
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    QMutexLocker locker(&CarpetDB::mutex());
    qDebug() << Q_FUNC_INFO << __LINE__;
    QVariantList ret;

    QSqlQuery query("SELECT excludePatternID"
                    ", pattern"
                    ", entireDirectoryIfExists"
                    " FROM ExcludePatterns", db);
    if (exec(query)) {
        while (query.next()) {
            QVariantMap item;
            item.insert("id", query.value(0).toInt());
            item.insert("pattern", query.value(1).toString());
            item.insert("entireDirectoryIfExists", query.value(2).toBool());
            ret.append(item);
        }
    }
    return ret;
}

void Backup::Private::addExcludePattern(const QString &pattern, bool entireDirectory)
{
    {
        AddExcludePatternJob *job = new AddExcludePatternJob(pattern, entireDirectory, this);
        job->setName(tr("add exclude pattern(%1)").arg(pattern));
        job->setMutex(&CarpetDB::mutex());
        connect(job, SIGNAL(finished()), this, SLOT(finished()));
        connect(job, SIGNAL(error(QString)), this, SLOT(error(QString)));
        if (jobQueue.isEmpty()) job->start();
        jobQueue.append(job);
    }
    {
        UpdateStatisticsJob *job = new UpdateStatisticsJob(this);
        job->setName(tr("Update statistics"));
        job->setMutex(&CarpetDB::mutex());
        connect(job, SIGNAL(finished()), this, SLOT(finished()));
        connect(job, SIGNAL(error(QString)), this, SLOT(error(QString)));
        if (jobQueue.isEmpty()) job->start();
        jobQueue.append(job);
    }
}

void Backup::Private::removeExcludePattern(const QString &id, const QString &pattern)
{
    {
        RemoveExcludePatternJob *job = new RemoveExcludePatternJob(id.toInt(), this);
        job->setName(tr("remove exclude pattern (%1)").arg(pattern));
        job->setMutex(&CarpetDB::mutex());
        connect(job, SIGNAL(finished()), this, SLOT(finished()));
        connect(job, SIGNAL(error(QString)), this, SLOT(error(QString)));
        if (jobQueue.isEmpty()) job->start();
        jobQueue.append(job);
    }
    {
        UpdateStatisticsJob *job = new UpdateStatisticsJob(this);
        job->setName(tr("Update statistics"));
        job->setMutex(&CarpetDB::mutex());
        connect(job, SIGNAL(finished()), this, SLOT(finished()));
        connect(job, SIGNAL(error(QString)), this, SLOT(error(QString)));
        if (jobQueue.isEmpty()) job->start();
        jobQueue.append(job);
    }
}

Backup::Backup(QObject *parent)
    : QObject(parent)
    , d(new Private(this))
{
    d->moveToThread(&thread);
    connect(d, SIGNAL(done(bool,QString)), this, SIGNAL(done(bool,QString)));
    thread.start(QThread::LowestPriority);
}

Backup::~Backup()
{
    thread.terminate();
    thread.wait();
    delete d;
}

bool Backup::enabled() const
{
    QSqlQuery query(d->db);
    query.prepare("SELECT value FROM Settings WHERE key = ?");
    query.addBindValue(QLatin1String("enabled"));
    if (d->exec(query)) {
        if (query.first()) {
            return query.value(0).toBool();
        } else {
            return false;
        }
    }
    return false;
}

void Backup::setEnabled(bool enabled)
{
    QSqlQuery query(d->db);
    query.prepare("SELECT value FROM Settings WHERE key = ?");
    query.addBindValue(QLatin1String("enabled"));
    if (d->exec(query)) {
        QSqlQuery query2(d->db);
        if (query.first()) {
            query2.prepare("UPDATE Settings SET value = ? WHERE key = ?");
        } else {
            query2.prepare("INSERT INTO Settings(value, key) VALUES(?, ?)");
        }
        query2.addBindValue(enabled);
        query2.addBindValue(QLatin1String("enabled"));
        if (d->exec(query2)) {
            emit enabledChanged(enabled);
        }
    }
}

Backup::Status Backup::status() const
{
    return d->status;
}

QVariantList Backup::jobList() const
{
    QVariantList ret;
    foreach (const AbstractCarpetJob *job, d->jobQueue) {
        QVariantMap m;
        m.insert("icon", job->icon());
        m.insert("name", job->name());
        ret.append(m);
    }
    return ret;
}

void Backup::backup(const QString &name)
{
    QMetaObject::invokeMethod(d, "backup", Qt::QueuedConnection, Q_ARG(QString, name));
}

void Backup::restore(int fileTreeId, const QString &to)
{
    d->restore(fileTreeId, to.isNull() ? d->from : to);
}

QVariantMap Backup::contents(const QString &path, const QString &backup) const
{
    return d->contents(path, backup);
}

QVariantMap Backup::diff(const QString &path, const QString &backup) const
{
    return d->diff(path, backup);
}

QVariantList Backup::tree() const
{
    return d->tree();
}

QVariantList Backup::excludePatterns() const
{
    return d->excludePatterns();
}

void Backup::addExcludePattern(const QString &pattern, bool entireDirectory)
{
    QMetaObject::invokeMethod(d, "addExcludePattern", Qt::QueuedConnection, Q_ARG(QString, pattern), Q_ARG(bool, entireDirectory));
}

void Backup::removeExcludePattern(const QString &id, const QString &pattern)
{
    QMetaObject::invokeMethod(d, "removeExcludePattern", Qt::QueuedConnection, Q_ARG(QString, id), Q_ARG(QString, pattern));
}

#include "backup.moc"

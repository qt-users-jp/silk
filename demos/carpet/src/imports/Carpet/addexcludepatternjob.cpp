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

#include "addexcludepatternjob.h"
#include "abstractcarpetjobprivate_p.h"

#include <QtCore/QMutexLocker>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QRegExp>

#include "silkconfig.h"

class AddExcludePatternJob::Private : public AbstractCarpetJob::Private
{
    Q_OBJECT

public:
    Private(const QString &pattern, bool entireDirectoryIfExists, AddExcludePatternJob *parent);

public slots:
    virtual void start();

private:
    void cleanup(int fileId);

private:
    AddExcludePatternJob *q;
    QString pattern;
    bool entireDirectoryIfExists;
};

AddExcludePatternJob::Private::Private(const QString &pattern, bool entireDirectoryIfExists, AddExcludePatternJob *parent)
    : AbstractCarpetJob::Private(parent)
    , q(parent)
    , pattern(pattern)
    , entireDirectoryIfExists(entireDirectoryIfExists)
{

}

void AddExcludePatternJob::Private::start()
{
    QMutexLocker lock(q->m_mutex);

    if (!db.transaction()) {
        QMetaObject::invokeMethod(q, "error", Qt::QueuedConnection, Q_ARG(QString, tr("database is locked.")));
        return;
    }

    // add pattern
    {
        QSqlQuery query(db);
        query.prepare(
                    "INSERT INTO ExcludePatterns("
                    "pattern"
                    ", entireDirectoryIfExists)"
                    " VALUES(?, ?)"
                    );
        query.addBindValue(pattern);
        query.addBindValue(entireDirectoryIfExists);
        if (!exec(query)) return;
    }

    {
        QSqlQuery query("CREATE INDEX IF NOT EXISTS BackupData_hash ON BackupData (hash)", db);
        if (!exec(query)) return;
    }

    // remove data that matche the pattern
    {
        QSqlQuery query(db);
        query.prepare(
                    "SELECT File.fileID"
                    ", File.fileName"
                    ", File.directoryID"
                    " FROM File"
                    " WHERE File.fileName LIKE ? ESCAPE '$'"
                    );
        QString ptn = pattern;
        ptn.replace("$", "$$");
        ptn.replace("%", "$%");
        ptn.replace("_", "$_");
        ptn.replace("*", "%");
        ptn.replace("?", "_");
        query.addBindValue(ptn);
        if (query.exec()) {
            while (query.next()) {
//                qDebug() << Q_FUNC_INFO << __LINE__ << query.value(1).toString();
                if (entireDirectoryIfExists) {
                    cleanup(query.value(2).toInt());
                } else {
                    cleanup(query.value(0).toInt());
                }
            }
        }
    }

    // TODO drop index
    {
        QSqlQuery query("DROP INDEX IF EXISTS BackupData_hash", db);
        if (!exec(query)) return;
    }

    if (db.commit()) {
        QMetaObject::invokeMethod(q, "finished", Qt::QueuedConnection);
    } else {
        error(tr("commit failed"));
    }
}

void AddExcludePatternJob::Private::cleanup(int fileId)
{
//    qDebug() << Q_FUNC_INFO << __LINE__ << fileId;
    // cleanup children
    {
        QSqlQuery query(db);
        query.prepare(
                    "SELECT fileID"
                    " FROM File"
                    " WHERE directoryID = ?"
                    );
        query.addBindValue(fileId);
        if (exec(query)) {
            while (query.next()) {
                cleanup(query.value(0).toInt());
            }
        }
    }

    // cleanup the file
    {
        QSqlQuery query(db);
        query.prepare(
                    "SELECT FileTree.fileTreeID"
                    ", BackupData.hash"
                    " FROM BackupData"
                    " INNER JOIN FileTree ON FileTree.fileTreeID = BackupData.fileTreeID"
                    " WHERE FileTree.fileID = ?"
                    );
        query.addBindValue(fileId);
        if (exec(query)) {
            while (query.next()) {
                int fileTreeId = query.value(0).toInt();
                QString hash = query.value(1).toString();
//                qDebug() << Q_FUNC_INFO << __LINE__ << hash;
                {
                    QSqlQuery query2(db);
                    query2.prepare("DELETE FROM FileTree WHERE fileTreeID = ?");
                    query2.addBindValue(fileTreeId);
                    exec(query2);
                }

                {
                    QSqlQuery query2(db);
                    query2.prepare("DELETE FROM BackupData WHERE fileTreeID = ?");
                    query2.addBindValue(fileTreeId);
                    exec(query2);
                }

                {
                    QSqlQuery query2(db);
                    query2.prepare("SELECT fileTreeId FROM BackupData WHERE hash = ?");
                    query2.addBindValue(hash);
                    if (exec(query2)) {
                        if (!query2.first()) {
//                            qDebug() << QString("%1/%2").arg(to).arg(hash.insert(2, "/"));
                            QFile::remove(QString("%1/%2").arg(backupTo).arg(hash.insert(2, "/")));
                        }
                    }
                }
            }
        }
    }
//    qDebug() << Q_FUNC_INFO << __LINE__;
}

AddExcludePatternJob::AddExcludePatternJob(const QString &pattern, bool entireDirectoryIfExists, QObject *parent)
    : AbstractCarpetJob(parent)
    , d(new Private(pattern, entireDirectoryIfExists, this))
{
}

void AddExcludePatternJob::start()
{
    QMetaObject::invokeMethod(d, "start", Qt::QueuedConnection);
}

#include "addexcludepatternjob.moc"

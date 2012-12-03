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

#include "updatestatisticsjob.h"
#include "abstractcarpetjobprivate_p.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QMutexLocker>
#include <QtCore/QVariant>

class UpdateStatisticsJob::Private : public AbstractCarpetJob::Private
{
    Q_OBJECT

public:
    enum Type {
        RecentlyChanged,
        FrequentlyChanged,
        SizeConsumed
    };

    Private(UpdateStatisticsJob *parent);

public slots:
    virtual void start();

private:
    bool updateRecentlyChanged();
    bool updateFrequentlyChanged();
    bool updateSizeConsumed();

private:
    UpdateStatisticsJob *q;
    Type type;
};

UpdateStatisticsJob::Private::Private(UpdateStatisticsJob *parent)
    : AbstractCarpetJob::Private(parent)
    , q(parent)
    , type(RecentlyChanged)
{
}

void UpdateStatisticsJob::Private::start()
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    QMutexLocker lock(q->m_mutex);
    qDebug() << Q_FUNC_INFO << __LINE__ << type;

    if (!db.transaction()) {
        QMetaObject::invokeMethod(q, "error", Qt::QueuedConnection, Q_ARG(QString, tr("database is locked.")));
        return;
    }

    switch (type) {
    case RecentlyChanged:
        if (!updateFrequentlyChanged()) return;
        if (db.commit()) {
            type = FrequentlyChanged;
            QMetaObject::invokeMethod(this, "start", Qt::QueuedConnection);
        } else {
            error(tr("commit failed"));
        }
        break;
    case FrequentlyChanged:
        if (!updateRecentlyChanged()) return;
        if (db.commit()) {
            type = SizeConsumed;
            QMetaObject::invokeMethod(this, "start", Qt::QueuedConnection);
        } else {
            error(tr("commit failed"));
        }
        break;
    case SizeConsumed:
        if (!updateSizeConsumed()) return;
        if (db.commit()) {
            QMetaObject::invokeMethod(q, "finished", Qt::QueuedConnection);
        } else {
            error(tr("commit failed"));
        }
        break;
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

bool UpdateStatisticsJob::Private::updateRecentlyChanged()
{
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM RecentlyChanged");
        if (!exec(query)) return false;
    }
    {
        QSqlQuery query(db);
        query.prepare("SELECT FileTree.fileID"
                      " FROM FileTree"
                      " INNER JOIN BackupData ON BackupData.fileTreeID = FileTree.fileTreeID"
                      " WHERE BackupData.hash IS NOT NULL"
                      " AND BackupData.cTime > ?"
                      " GROUP BY FileTree.fileID"
                      " ORDER BY BackupData.cTime DESC"
                      " LIMIT 100");
        QDateTime oneWeekAgo = QDateTime::currentDateTime();
        query.addBindValue(oneWeekAgo.addDays(-1));
        if (!exec(query)) return false;
        int i = 0;
        while (query.next()) {
            QSqlQuery query2(db);
            query2.prepare("INSERT INTO RecentlyChanged(sequence, fileID) VALUES(?, ?)");
            query2.addBindValue(i++);
            query2.addBindValue(query.value(0).toInt());
            if (!exec(query2)) return false;
        }
    }
    return true;
}

bool UpdateStatisticsJob::Private::updateFrequentlyChanged()
{
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM FrequentlyChanged");
        if (!exec(query)) return false;
    }
    {
        QSqlQuery query(db);
        query.prepare("SELECT FileTree.fileID"
                      ", count(FileTree.backupID) AS backupCount"
                      " FROM FileTree"
                      " INNER JOIN BackupData ON BackupData.fileTreeID = FileTree.fileTreeID"
                      " WHERE BackupData.hash IS NOT NULL"
                      " GROUP BY FileTree.fileID"
                      " ORDER BY backupCount DESC"
                      " LIMIT 100");
        if (!exec(query)) return false;
        int i = 0;
        while (query.next()) {
            QSqlQuery query2(db);
            query2.prepare("INSERT INTO FrequentlyChanged(sequence, fileID) VALUES(?, ?)");
            query2.addBindValue(i++);
            query2.addBindValue(query.value(0).toInt());
            if (!exec(query2)) return false;
        }
    }
    return true;
}

bool UpdateStatisticsJob::Private::updateSizeConsumed()
{
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM SizeConsumed");
        if (!exec(query)) return false;
    }
    {
        QSqlQuery query(db);
        query.prepare("SELECT FileSize.fileID"
                      ", sum(FileSize.size) as backupSize"
                      " FROM ("
                          "SELECT FileTree.fileID"
                          ", BackupData.size"
                          " FROM FileTree"
                          " INNER JOIN BackupData ON BackupData.fileTreeID = FileTree.fileTreeID"
                          " WHERE BackupData.hash IS NOT NULL"
                          " GROUP BY FileTree.fileID"
                          ", BackupData.size"
                          ", BackupData.hash"
                      ") AS FileSize"
                      " GROUP BY FileSize.fileID"
                      " ORDER BY backupSize DESC"
                      " LIMIT 100");
        if (!exec(query)) return false;
        int i = 0;
        while (query.next()) {
            QSqlQuery query2(db);
            query2.prepare("INSERT INTO SizeConsumed(sequence, fileID) VALUES(?, ?)");
            query2.addBindValue(i++);
            query2.addBindValue(query.value(0).toInt());
            if (!exec(query2)) return false;
        }
    }
    return true;
}

UpdateStatisticsJob::UpdateStatisticsJob(QObject *parent)
    : AbstractCarpetJob(parent)
    , d(new Private(this))
{
}

void UpdateStatisticsJob::start()
{
    QMetaObject::invokeMethod(d, "start", Qt::QueuedConnection);
}

#include "updatestatisticsjob.moc"

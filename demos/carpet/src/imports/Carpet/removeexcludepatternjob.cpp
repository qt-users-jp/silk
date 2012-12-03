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

#include "removeexcludepatternjob.h"
#include "abstractcarpetjobprivate_p.h"

#include <QtCore/QMutexLocker>
#include <QtCore/QDebug>

class RemoveExcludePatternJob::Private : public AbstractCarpetJob::Private
{
    Q_OBJECT

public:
    Private(int id, RemoveExcludePatternJob *parent);

public slots:
    virtual void start();

private:
    RemoveExcludePatternJob *q;
    int id;
};

RemoveExcludePatternJob::Private::Private(int id, RemoveExcludePatternJob *parent)
    : AbstractCarpetJob::Private(parent)
    , q(parent)
    , id(id)
{

}

void RemoveExcludePatternJob::Private::start()
{
    QMutexLocker lock(q->m_mutex);

    if (!db.transaction()) {
        QMetaObject::invokeMethod(q, "error", Qt::QueuedConnection, Q_ARG(QString, tr("database is locked.")));
        return;
    }
    // remove pattern
    {
        QSqlQuery query(db);
        query.prepare(
                    "DELETE FROM ExcludePatterns"
                    " WHERE excludePatternID = ?"
                    );
        query.addBindValue(id);
        if (!exec(query)) return;
    }

    if (db.commit()) {
        QMetaObject::invokeMethod(q, "finished", Qt::QueuedConnection);
    } else {
        error(tr("commit failed"));
    }
}

RemoveExcludePatternJob::RemoveExcludePatternJob(int id, QObject *parent)
    : AbstractCarpetJob(parent)
    , d(new Private(id, this))
{
}

void RemoveExcludePatternJob::start()
{
    QMetaObject::invokeMethod(d, "start", Qt::QueuedConnection);
}

#include "removeexcludepatternjob.moc"

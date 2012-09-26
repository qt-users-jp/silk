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

#include "database.h"

#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtQml/qqml.h>
#include <QtQml/QQmlContext>

class Database::Private : public QObject
{
    Q_OBJECT
public:
    Private(Database *parent);

    void open();

private:
    Database *q;
};

Database::Private::Private(Database *parent)
    : QObject(parent)
    , q(parent)
{
}

void Database::Private::open()
{
    if (q->m_connectionName.isEmpty()) {
        QQmlContext *context = qmlContext(q);
        q->connectionName(context->nameForObject(q));
    }

    if (!QSqlDatabase::contains(q->m_connectionName)) {
        QSqlDatabase db = QSqlDatabase::addDatabase(q->m_type, q->m_connectionName);
        db.setHostName(q->m_hostName);
        db.setDatabaseName(q->m_databaseName);
        db.setUserName(q->m_userName);
        db.setPassword(q->m_password);
        if (db.open()) {
            q->open(true);
        } else {
            qDebug() << Q_FUNC_INFO << __LINE__ << db.lastError().text();
            q->open(false);
        }
    } else {
        q->open(QSqlDatabase::database(q->m_connectionName).isOpen());
    }
}

Database::Database(QObject *parent)
    : SilkAbstractObject(parent)
    , m_open(false)
    , d(new Private(this))
{
}

bool Database::transaction()
{
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);
    return db.transaction();
}

bool Database::commit()
{
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);
    return db.commit();
}

bool Database::rollback()
{
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);
    return db.rollback();
}

void Database::classBegin()
{
}

void Database::componentComplete()
{
    d->open();
}

#include "database.moc"

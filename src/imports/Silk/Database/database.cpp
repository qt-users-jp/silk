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
#include <QtCore/QTimer>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtQml/qqml.h>
#include <QtQml/QQmlContext>

class Database::Private : public QObject
{
    Q_OBJECT
public:
    Private(Database *parent);

public slots:
    void open();
private slots:
    void changed();

private:
    Database *q;
    QTimer timer;
};

Database::Private::Private(Database *parent)
    : QObject(parent)
    , q(parent)
{
    timer.setInterval(0);
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(open()));
    connect(q, SIGNAL(connectionNameChanged(QString)), this, SLOT(changed()));
    connect(q, SIGNAL(typeChanged(QString)), this, SLOT(changed()));
    connect(q, SIGNAL(databaseNameChanged(QString)), this, SLOT(changed()));
    connect(q, SIGNAL(hostNameChanged(QString)), this, SLOT(changed()));
    connect(q, SIGNAL(userNameChanged(QString)), this, SLOT(changed()));
    connect(q, SIGNAL(passwordChanged(QString)), this, SLOT(changed()));
}

void Database::Private::changed()
{
    if (!timer.isActive()) timer.start();
}

void Database::Private::open()
{
    if (q->m_databaseName.isNull()) return;
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
        }
    } else {
        q->open(QSqlDatabase::database(q->m_connectionName).isOpen());
    }
}

Database::Database(QObject *parent)
    : SilkAbstractObject(parent)
    , m_hostName("localhost")
    , m_open(false)
    , d(new Private(this))
{
}

bool Database::open()
{
    if (!m_open) {
        d->open();
    }
    return m_open;
}

void Database::open(bool open)
{
    if (m_open == open) return;
    m_open = open;
    emit openChanged(open);
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

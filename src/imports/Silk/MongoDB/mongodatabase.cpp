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

#include "mongodatabase.h"

#include <QtCore/QDebug>

#include "mongo.h"

class MongoDatabase::Private
{
public:
    Private();
    mongo connection[1];
    bool open;
    bool authenticated;
};

MongoDatabase::Private::Private()
    : open(false)
    , authenticated(false)
{}

MongoDatabase::MongoDatabase(QObject *parent)
    : SilkAbstractObject(parent)
    , d(new Private)
    , m_port(27017)
{
}

MongoDatabase::~MongoDatabase()
{
    delete d;
}

void MongoDatabase::classBegin()
{
}

void MongoDatabase::componentComplete()
{
    open();
}

bool MongoDatabase::open()
{
    if (!d->open) {
        mongo_init(d->connection);
        int status = mongo_client(d->connection, m_host.toUtf8().constData(), m_port);

        if (status != MONGO_OK) {
            switch (d->connection->err) {
            case MONGO_CONN_NO_SOCKET:
                break;
            default:
                break;
            }
            qWarning() << Q_FUNC_INFO << __LINE__ << d->connection->err;
            qWarning() << Q_FUNC_INFO << __LINE__ << QString::fromUtf8(d->connection->errstr);
            mongo_destroy(d->connection);
        } else {
            open(true);
        }
    }
    return d->open;
}

void MongoDatabase::open(bool open)
{
    if (d->open == open) return;
    d->open = open;
    emit openChanged(open);
}

bool MongoDatabase::authenticated()
{
    if (!d->authenticated && d->open) {
        if (m_user.isEmpty()) {
            authenticated(true);
        } else {
            int status = mongo_cmd_authenticate(d->connection, m_name.toUtf8().constData(), m_user.toUtf8().constData(), m_pass.toUtf8().constData());
            if (status != MONGO_OK) {

                qWarning() << Q_FUNC_INFO << __LINE__ << d->connection->err;
                qWarning() << Q_FUNC_INFO << __LINE__ << QString::fromUtf8(d->connection->errstr);
            } else {
                authenticated(true);
            }
        }
    }
    return d->authenticated;
}

void MongoDatabase::authenticated(bool authenticated)
{
    if (d->authenticated == authenticated) return;
    d->authenticated = authenticated;
    emit authenticatedChanged(authenticated);
}

mongo *MongoDatabase::connection()
{
    return d->connection;
}

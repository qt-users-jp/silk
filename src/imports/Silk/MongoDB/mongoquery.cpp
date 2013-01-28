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

#include "mongoquery.h"
#include "mongocollection.h"
#include "mongodatabase.h"

#include <QtCore/QDebug>
#include <QtCore/QStringList>

#include "mongo.h"

class MongoQuery::Private
{
public:
    QList<QVariantMap> data;
    QHash<int, QByteArray> roleNames;
};

static bool object2bson(const QVariantMap &from, bson *to)
{
    bool ret = true;
    QVariantMap::const_iterator i = from.constBegin();
    while (i != from.constEnd()) {
        const char *key = i.key().toUtf8().constData();
        switch (i.value().type()) {
        case QVariant::Int:
            bson_append_int(to, key, i.value().toInt());
            break;
        case QVariant::Bool:
            bson_append_bool(to, key, i.value().toBool());
            break;
        case QVariant::Map: {
            bson_append_start_object(to, key);
            object2bson(i.value().toMap(), to);
            bson_append_finish_object(to);
            break; }
        default:
            qWarning() << Q_FUNC_INFO << __LINE__ << i.value() << "not supported";
            break;
        }

        ++i;
    }

    return ret;
}

static QVariantMap bson2object(const bson *b)
{
    bson_iterator iterator[1];
    bson_iterator_init(iterator, b);
    QVariantMap ret;
    if (bson_iterator_more(iterator)) {
        bool eoo = false;
        while(!eoo) {
            bson_type type = bson_iterator_next(iterator);
            const char *key = bson_iterator_key(iterator);
    //        qDebug() << Q_FUNC_INFO << __LINE__ << type << key;
            switch (type) {
            case BSON_EOO:
                eoo = true;
                break;
            case BSON_DOUBLE:
                ret.insert(key, bson_iterator_double(iterator));
                break;
            case BSON_STRING:
                ret.insert(key, bson_iterator_string(iterator));
                break;
            case BSON_OBJECT: {
                bson sub[1];
                bson_iterator_subobject(iterator, sub);
                ret.insert(key, bson2object(sub));
                break; }
//            case BSON_ARRAY: {
//                bson sub[1];
//                bson_iterator_subobject(iterator, sub);
//                ret.insert(key, bson2list(sub));
//                break; }
            case BSON_OID: {
                char oid[25];
                bson_oid_to_string(bson_iterator_oid(iterator), oid);
                ret.insert(key, oid);
                break; }
            case BSON_INT:
                ret.insert(key, bson_iterator_int(iterator));
                break;
            default:
                qDebug() << type << key;
                break;
            }
        }
    }    return ret;
}

MongoQuery::MongoQuery(QObject *parent)
    : QAbstractListModel(parent)
    , d(new Private)
    , m___collection(qobject_cast<MongoCollection *>(parent))
    , m___skip(0)
    , m___limit(0)
{
}

MongoQuery::~MongoQuery()
{
    delete d;
}

MongoQuery *MongoQuery::sort(const QVariantMap &sort)
{
    __sort(sort);
    return this;
}

MongoQuery *MongoQuery::skip(int n)
{
    __skip(n);
    return this;
}

MongoQuery *MongoQuery::limit(int n)
{
    __limit(n);
    return this;
}

void MongoQuery::read()
{
    MongoDatabase *db = m___collection->database();

    if (!db->open()) {
        qDebug() << Q_FUNC_INFO << __LINE__;
        return;
    }
    if (!db->authenticated()) {
        qDebug() << Q_FUNC_INFO << __LINE__;
        return;
    }

    mongo *conn = db->connection();

    QString ns = QString("%1.%2").arg(db->name()).arg(m___collection->name());

    mongo_cursor cursor[1];
    mongo_cursor_init(cursor, conn, ns.toUtf8().constData());

    QVariantMap map;
    map.insert(QLatin1String("$query"), m___query);
    map.insert(QLatin1String("$orderby"), m___sort);
    bson mongo_query[1];
    bson_init(mongo_query);
    object2bson(map, mongo_query);
    bson_finish(mongo_query);
    mongo_cursor_set_query(cursor, mongo_query);

    bson mongo_fields[1];
    bson_init(mongo_fields);
    object2bson(m___fields, mongo_fields);
    bson_finish(mongo_fields);
    mongo_cursor_set_fields(cursor, mongo_fields);

    mongo_cursor_set_skip(cursor, m___skip);
    mongo_cursor_set_limit(cursor, m___limit);

    int count = 0;
    int role = Qt::UserRole;
    QStringList keys;
    while (mongo_cursor_next(cursor) == MONGO_OK) {
        QVariantMap map = bson2object(&cursor->current);
        foreach (const QString &key, map.keys()) {
            if (!keys.contains(key)) {
                keys.append(key);
                d->roleNames.insert(role++, key.toUtf8());
            }
        }

        d->data.append(map);
    }
    beginInsertRows(QModelIndex(), 0, count - 1);
    endInsertRows();
    mongo_cursor_destroy(cursor);
}

int MongoQuery::rowCount(const QModelIndex &parent) const
{
    return d->data.count();
}

QVariant MongoQuery::data(const QModelIndex &index, int role) const
{
    if (role >= Qt::UserRole) {
        return d->data.at(index.row()).value(d->roleNames.value(role));
    }
    return QVariant();
}

QHash<int, QByteArray> MongoQuery::roleNames() const
{
    return d->roleNames;
}

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

#ifndef MONGOQUERY_H
#define MONGOQUERY_H

#include <QtCore/QAbstractListModel>

class MongoCollection;

#define ADD_PROPERTY(type, name, type2) \
public: \
    type name() const { return m_##name; } \
    void name(type name) { \
        if (m_##name == name) return; \
        m_##name = name; \
        emit name##Changed(name); \
    } \
private: \
    type2 m_##name;

class MongoQuery : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(MongoCollection *__collection READ __collection WRITE __collection NOTIFY __collectionChanged)
    Q_PROPERTY(QVariantMap __query READ __query WRITE __query NOTIFY __queryChanged)
    Q_PROPERTY(QVariantMap __fields READ __fields WRITE __fields NOTIFY __fieldsChanged)
    Q_PROPERTY(QVariantMap __sort READ __sort WRITE __sort NOTIFY __sortChanged)
    Q_PROPERTY(int __skip READ __skip WRITE __skip NOTIFY __skipChanged)
    Q_PROPERTY(int __limit READ __limit WRITE __limit NOTIFY __limitChanged)
public:
    explicit MongoQuery(QObject *parent = 0);
    ~MongoQuery();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE MongoQuery *sort(const QVariantMap &sort);
    Q_INVOKABLE MongoQuery *skip(int n);
    Q_INVOKABLE MongoQuery *limit(int n);

public slots:
    void read();

signals:
    void __collectionChanged(MongoCollection *__collection);
    void __queryChanged(const QVariantMap &__query);
    void __fieldsChanged(const QVariantMap &__fields);
    void __sortChanged(const QVariantMap &__sort);
    void __skipChanged(int __skip);
    void __limitChanged(int __limit);

private:
    class Private;
    Private *d;

    ADD_PROPERTY(MongoCollection *, __collection, MongoCollection *)
    ADD_PROPERTY(const QVariantMap &, __query, QVariantMap)
    ADD_PROPERTY(const QVariantMap &, __fields, QVariantMap)
    ADD_PROPERTY(const QVariantMap &, __sort, QVariantMap)
    ADD_PROPERTY(int, __skip, int)
    ADD_PROPERTY(int, __limit, int)
};

#endif // MONGOQUERY_H

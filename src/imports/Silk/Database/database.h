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

#ifndef DATABASE_H
#define DATABASE_H

#include <silkabstractobject.h>

#include <QtCore/QDebug>

#include <QtQml/QQmlParserStatus>

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

#define ADD_DB_PROPERTY(type, name, type2) \
public: \
    type name() const { return m_##name; } \
    void name(type name) { \
        if (m_##name == name) return; \
        if (m_open) { \
            qWarning() << "You are not able to change " << #name << "because database is open."; \
            return; \
        } \
        m_##name = name; \
        emit name##Changed(name); \
    } \
private: \
    type2 m_##name;

class Database : public SilkAbstractObject, public QQmlParserStatus
{
    Q_OBJECT

    Q_PROPERTY(QString connectionName READ connectionName WRITE connectionName NOTIFY connectionNameChanged)
    ADD_DB_PROPERTY(const QString &, connectionName, QString)
    Q_PROPERTY(QString type READ type WRITE type NOTIFY typeChanged)
    ADD_DB_PROPERTY(const QString &, type, QString)
    Q_PROPERTY(QString databaseName READ databaseName WRITE databaseName NOTIFY databaseNameChanged)
    ADD_DB_PROPERTY(const QString &, databaseName, QString)
    Q_PROPERTY(QString hostName READ hostName WRITE hostName NOTIFY hostNameChanged)
    ADD_DB_PROPERTY(const QString &, hostName, QString)
    Q_PROPERTY(QString userName READ userName WRITE userName NOTIFY userNameChanged)
    ADD_DB_PROPERTY(const QString &, userName, QString)
    Q_PROPERTY(QString password READ password WRITE password NOTIFY passwordChanged)
    ADD_DB_PROPERTY(const QString &, password, QString)

    Q_PROPERTY(bool open READ open WRITE open NOTIFY openChanged)
    ADD_PROPERTY(bool, open, bool)

    Q_INTERFACES(QQmlParserStatus)
public:
    explicit Database(QObject *parent = 0);

    Q_INVOKABLE bool transaction();
    Q_INVOKABLE bool commit();
    Q_INVOKABLE bool rollback();

    virtual void classBegin();
    virtual void componentComplete();

signals:
    void connectionNameChanged(const QString &connectionName);
    void typeChanged(const QString &type);
    void databaseNameChanged(const QString &databaseName);
    void hostNameChanged(const QString &hostName);
    void userNameChanged(const QString &userName);
    void passwordChanged(const QString &password);
    void openChanged(bool open);
    void transactionChanged(bool transaction);

private:
    class Private;
    Private *d;
};

#endif // DATABASE_H

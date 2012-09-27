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

#ifndef SMTP_H
#define SMTP_H

#include <QtCore/QObject>
#include <QtCore/QVariantMap>

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

class Smtp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString host READ host WRITE host NOTIFY hostChanged)
    Q_PROPERTY(QString encrypt READ encrypt WRITE encrypt NOTIFY encryptChanged)
    Q_PROPERTY(int port READ port WRITE port NOTIFY portChanged)
    Q_PROPERTY(QString username READ username WRITE username NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE password NOTIFY passwordChanged)
    Q_PROPERTY(QString from READ from WRITE from NOTIFY fromChanged)

public:
    explicit Smtp(QObject *parent = 0);

public slots:
    void send(const QVariantMap &email);

signals:
    void hostChanged(const QString &host);
    void encryptChanged(const QString &encrypt);
    void portChanged(int port);
    void usernameChanged(const QString &username);
    void passwordChanged(const QString &password);
    void fromChanged(const QString &from);

    void sent(const QVariantMap &email);
    void error(const QVariantMap &email, const QString &message);

private:
    class Private;
//    Private *d;

    ADD_PROPERTY(const QString &, host, QString)
    ADD_PROPERTY(const QString &, encrypt, QString)
    ADD_PROPERTY(int, port, int)
    ADD_PROPERTY(const QString &, username, QString)
    ADD_PROPERTY(const QString &, password, QString)
    ADD_PROPERTY(const QString &, from, QString)
};

#endif // SMTP_H

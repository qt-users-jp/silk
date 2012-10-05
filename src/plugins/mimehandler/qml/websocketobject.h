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

#ifndef WEBSOCKETOBJECT_H
#define WEBSOCKETOBJECT_H

#include <silkabstractobject.h>
#include <qwebsocket.h>

class WebSocketObject : public SilkAbstractObject
{
    Q_OBJECT

    Q_PROPERTY(QString remoteAddress READ remoteAddress NOTIFY remoteAddressChanged)
    SILK_ADD_PROPERTY(const QString &, remoteAddress, QString)
    Q_PROPERTY(QString scheme READ scheme NOTIFY schemeChanged)
    SILK_ADD_PROPERTY(const QString &, scheme, QString)
    Q_PROPERTY(QString host READ host NOTIFY hostChanged)
    SILK_ADD_PROPERTY(const QString &, host, QString)
    Q_PROPERTY(int port READ port NOTIFY portChanged)
    SILK_ADD_PROPERTY(int, port, int)
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
    SILK_ADD_PROPERTY(const QString &, path, QString)
    Q_PROPERTY(QString query READ query NOTIFY queryChanged)
    SILK_ADD_PROPERTY(const QString &, query, QString)
    Q_PROPERTY(QVariant requestHeader READ requestHeader NOTIFY requestHeaderChanged)
    SILK_ADD_PROPERTY(const QVariant &, requestHeader, QVariant)
    Q_PROPERTY(QVariantMap requestCookies READ requestCookies NOTIFY requestCookiesChanged)
    SILK_ADD_PROPERTY(const QVariantMap &, requestCookies, QVariantMap)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
    SILK_ADD_PROPERTY(const QString &, message, QString)

public:
    explicit WebSocketObject(QObject *parent = 0);

    void setWebSocket(QWebSocket *socket);
public slots:
    void accept(const QByteArray &protocol = QByteArray());
    void send(const QByteArray &data);

signals:
    void message(const QVariantMap &message);
    void ready();
    void remoteAddressChanged(const QString &remoteAddress);
    void schemeChanged(const QString &scheme);
    void hostChanged(const QString &host);
    void portChanged(int port);
    void pathChanged(const QString &path);
    void queryChanged(const QString &query);
    void requestHeaderChanged(const QVariant &requestHeader);
    void requestCookiesChanged(const QVariantMap &requestHeader);
    void messageChanged(const QString &message);

private slots:
    void onmessage(const QByteArray &msg);

private:
    QWebSocket *m_socket;
};

#endif // WEBSOCKETOBJECT_H

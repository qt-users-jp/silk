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

#ifndef SILKHTTPOBJECT_H
#define SILKHTTPOBJECT_H

#include <silkabstracthttpobject.h>

#include <QtCore/QUrl>

class SilkHttpObject : public SilkAbstractHttpObject
{
    Q_OBJECT
    Q_PROPERTY(QString method READ method NOTIFY methodChanged)
    SILK_ADD_PROPERTY(const QString &, method, QString)
    Q_PROPERTY(QUrl url READ url NOTIFY urlChanged)
    SILK_ADD_PROPERTY(const QUrl &, url, QUrl)
    Q_PROPERTY(QVariant requestHeader READ requestHeader NOTIFY requestHeaderChanged)
    SILK_ADD_PROPERTY(const QVariant &, requestHeader, QVariant)
    Q_PROPERTY(QVariantMap requestCookies READ requestCookies NOTIFY requestCookiesChanged)
    SILK_ADD_PROPERTY(const QVariantMap &, requestCookies, QVariantMap)
    Q_PROPERTY(QString query READ query NOTIFY queryChanged)
    SILK_ADD_PROPERTY(const QString &, query, QString)
    Q_PROPERTY(QString data READ data NOTIFY dataChanged)
    SILK_ADD_PROPERTY(const QString &, data, QString)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
    SILK_ADD_PROPERTY(const QString &, message, QString)

    Q_PROPERTY(bool loading READ loading WRITE loading NOTIFY loadingChanged)
    SILK_ADD_PROPERTY(bool, loading, bool)
    Q_PROPERTY(int status READ status WRITE status NOTIFY statusChanged)
    SILK_ADD_PROPERTY(int, status, int)
    Q_PROPERTY(QVariantMap responseHeader READ responseHeader WRITE responseHeader NOTIFY responseHeaderChanged)
    SILK_ADD_PROPERTY(const QVariantMap &, responseHeader, QVariantMap)
    Q_PROPERTY(QVariantMap responseCookies READ responseCookies WRITE responseCookies NOTIFY responseCookiesChanged)
    SILK_ADD_PROPERTY(const QVariantMap &, responseCookies, QVariantMap)
    Q_PROPERTY(bool escape READ escape WRITE escape NOTIFY escapeChanged)
    SILK_ADD_PROPERTY(bool, escape, bool)
public:
    explicit SilkHttpObject(QObject *parent = 0);

    virtual QByteArray out() const;

signals:
    void ready();
    void methodChanged(const QString &method);
    void urlChanged(const QUrl &url);
    void requestHeaderChanged(const QVariant &requestHeader);
    void requestCookiesChanged(const QVariantMap &requestHeader);
    void queryChanged(const QString &query);
    void dataChanged(const QString &data);
    void messageChanged(const QString &message);
    void loadingChanged(bool loading);
    void statusChanged(int status);
    void responseHeaderChanged(const QVariantMap &responseHeader);
    void responseCookiesChanged(const QVariantMap &responseHeader);
    void escapeChanged(bool escape);
};

#endif // SILKHTTPOBJECT_H

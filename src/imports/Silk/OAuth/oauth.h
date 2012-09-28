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

#ifndef OAUTH_H
#define OAUTH_H

#include <silkabstractobject.h>
#include <QtCore/QMultiMap>
#include <QtCore/QUrl>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

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

class OAuth : public SilkAbstractObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl requestTokenUrl READ requestTokenUrl WRITE requestTokenUrl NOTIFY requestTokenUrlChanged)
    ADD_PROPERTY(const QUrl &, requestTokenUrl, QUrl)
    Q_PROPERTY(QUrl authorizeUrl READ authorizeUrl WRITE authorizeUrl NOTIFY authorizeUrlChanged)
    ADD_PROPERTY(const QUrl &, authorizeUrl, QUrl)
    Q_PROPERTY(QUrl accessTokenUrl READ accessTokenUrl WRITE accessTokenUrl NOTIFY accessTokenUrlChanged)
    ADD_PROPERTY(const QUrl &, accessTokenUrl, QUrl)

    Q_PROPERTY(QString consumerKey READ consumerKey WRITE consumerKey NOTIFY consumerKeyChanged)
    ADD_PROPERTY(const QString &, consumerKey, QString)
    Q_PROPERTY(QString consumerSecret READ consumerSecret WRITE consumerSecret NOTIFY consumerSecretChanged)
    ADD_PROPERTY(const QString &, consumerSecret, QString)
    Q_PROPERTY(QString token READ token WRITE token NOTIFY tokenChanged)
    ADD_PROPERTY(const QString &, token, QString)
    Q_PROPERTY(QString tokenSecret READ tokenSecret WRITE tokenSecret NOTIFY tokenSecretChanged)
    ADD_PROPERTY(const QString &, tokenSecret, QString)
    Q_PROPERTY(bool authorized READ authorized NOTIFY authorizedChanged)
    ADD_PROPERTY(bool, authorized, bool)
public:
    enum State {
        Unauthorized,
        ObtainUnauthorizedRequestToken,
        RequestTokenReceived,
        UserAuthorizesRequestToken,
        ExchangeRequestTokenForAccessToken,
        Authorized
    };
    enum AuthorizeBy {
        AuthorizeByHeader,
        AuthorizeByBody,
        AuthorizeByUrl
    };

    explicit OAuth(QObject *parent = 0);

    QNetworkAccessManager *networkAccessManager() const;
    void setNetworkAccessManager(QNetworkAccessManager *networkAccessManager);

    AuthorizeBy authorizeBy() const;
    void setAuthorizeBy(AuthorizeBy authorizeBy);

    Q_INVOKABLE QString authHeader(const QString &method, const QUrl &url, const QVariantMap &params = QVariantMap());
    Q_INVOKABLE QString sign(const QString &method, const QUrl &url, const QVariantMap &params = QVariantMap());

public slots:
    void requestToken();
    void accessToken(const QString &verifier);

signals:
    void authorize(const QUrl &url);

    void requestTokenUrlChanged(const QUrl &requestTokenUrl);
    void authorizeUrlChanged(const QUrl &authorizeUrl);
    void accessTokenUrlChanged(const QUrl &accessTokenUrl);

    void consumerKeyChanged(const QString &consumerKey);
    void consumerSecretChanged(const QString &consumerSecret);
    void tokenChanged(const QString &token);
    void tokenSecretChanged(const QString &tokenSecret);
    void authorizedChanged(bool isAuthorized);

    void networkAccessManagerChanged(QNetworkAccessManager *networkAccessManager);

private:
    class Private;
    Private *d;
};

#endif // OAUTH_H

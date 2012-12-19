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

#include "oauth.h"
#include "hmac_sha1.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class OAuth::Private : public QObject {
    Q_OBJECT
public:
    Private(OAuth *parent);

    void requestToken();
    void accessToken(const QString &verifier);

    QNetworkReply *request(const QString &method, const QUrl &url, const QMultiMap<QString, QByteArray> &params, bool multiPart, const QByteArray &acceptEncoding = QByteArray());
    QString sign(const QString &method, const QUrl &url, const QVariantMap &params);
    QString authHeader(const QString &method, const QUrl &url, const QVariantMap &params);
    bool updateToken(QNetworkReply* rep);

    QMap<QString, QByteArray> requestParams;

    QNetworkAccessManager *networkAccessManager;
    AuthorizeBy authorizeBy;

private slots:
    void requestTokenFinished();
    void accessTokenFinished();
    void error(QNetworkReply::NetworkError err);
    void setToken(const QString &token);

private:
    OAuth *q;

    QByteArray parameterEncoding(const QString &str) const;
    QByteArray normalize(const QMultiMap<QString, QByteArray> &param) const;
    QByteArray signature(const QString &method, const QUrl &url, const QByteArray &params) const;
    QMultiMap<QString, QByteArray> signatureParams(const QMultiMap<QString, QByteArray> &params) const;
    QByteArray authHeader(const QMultiMap<QString, QByteArray> &params, const QUrl &realm = QUrl()) const;
    void getTokenCredential();
};

OAuth::Private::Private(OAuth *parent)
    : QObject(parent)
    , networkAccessManager(0)
    , authorizeBy(AuthorizeByHeader)
    , q(parent)
{
    connect(q, SIGNAL(tokenChanged(QString)), this, SLOT(setToken(QString)));
}

void OAuth::Private::requestToken()
{
    QMultiMap<QString, QByteArray> params;

    q->setAuthorizeBy(AuthorizeByHeader);
    QNetworkReply *reply = request(QLatin1String("POST"), q->m_requestTokenUrl, params, false);
    connect(reply, SIGNAL(finished()), this, SLOT(requestTokenFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

void OAuth::Private::requestTokenFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (updateToken(reply)) {
        emit q->authorize(QUrl(QString("%1?oauth_token=%2").arg(q->m_authorizeUrl.toString()).arg(q->m_token)));
    }
    reply->deleteLater();
}

void OAuth::Private::accessToken(const QString &verifier)
{
    QMultiMap<QString, QByteArray> params;
    params.insert(QLatin1String("oauth_verifier"), verifier.toUtf8());

    q->setAuthorizeBy(AuthorizeByHeader);
    QNetworkReply *reply = request(QLatin1String("POST"), q->m_accessTokenUrl, params, false);
    connect(reply, SIGNAL(finished()), this, SLOT(accessTokenFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

void OAuth::Private::accessTokenFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (updateToken(reply)) {
        q->authorized(true);
    }
    reply->deleteLater();
}

void OAuth::Private::error(QNetworkReply::NetworkError err)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qDebug() << Q_FUNC_INFO << __LINE__ << reply->url() << err;
    reply->deleteLater();
}

void OAuth::Private::setToken(const QString &token)
{
    if (token.isEmpty()) {
        requestParams.clear();
    } else if (!requestParams.isEmpty()){
        requestParams["oauth_token"] = token.toUtf8();
    }
}

QNetworkReply *OAuth::Private::request(const QString &method, const QUrl &url, const QMultiMap<QString, QByteArray> &params, bool multiPart, const QByteArray &acceptEncoding)
{
//    qDebug() << Q_FUNC_INFO << __LINE__ << method << url << params << multiPart;
    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setRawHeader("Accept-Encoding",acceptEncoding);

    QNetworkReply *ret = 0;

    QDateTime dt = QDateTime::currentDateTimeUtc();
    QString timestamp;
    QString nonce;
    timestamp.setNum(dt.toTime_t());
    nonce.setNum(dt.toMSecsSinceEpoch());

    if (requestParams.isEmpty()) {
        requestParams["oauth_version"] = "1.0";
        requestParams["oauth_signature_method"] = "HMAC-SHA1";
        requestParams["oauth_consumer_key"] = q->m_consumerKey.toUtf8();
        if (!q->m_token.isEmpty())
            requestParams["oauth_token"] = q->m_token.toUtf8();
        if (!q->m_callbackUrl.isEmpty())
            requestParams["oauth_callback"] = q->m_callbackUrl.toString().toUtf8();
//        qDebug() << QUrl::toPercentEncoding(q->m_callbackUrl.toString());
    }
    requestParams["oauth_nonce"] = nonce.toUtf8();
    requestParams["oauth_timestamp"] = timestamp.toUtf8();

    if (url.host() == "upload.twitter.com") {
        requestParams["oauth_signature"] =  signature(method,
                                                      url,
                                                      normalize(signatureParams(QMultiMap<QString, QByteArray>())));
    } else {
        requestParams["oauth_signature"] =  signature(method,
                                                      url,
                                                      normalize(signatureParams(params)));
    }

//    qDebug() << Q_FUNC_INFO << __LINE__ << requestParams;

    if (method == "POST") {
        switch (authorizeBy) {
        case AuthorizeByHeader:
            if (multiPart) {
                request.setRawHeader("Authorization", authHeader(QMultiMap<QString, QByteArray>()));
                request.setUrl(url);
                QString boundary = QString("--------------------") + nonce;
                QByteArray body;
                QList<QString> keys = params.keys();
                foreach(const QString &key, keys) {
                    QList<QByteArray> vals = params.values(key);
                    foreach(const QByteArray &val, vals) {
                        body.append(QString("--%1\r\n").arg(boundary).toUtf8());
                        if (key == "media[]") {
                            QUrl url(val);
                            QFileInfo info(url.path());
                            QFile file(info.filePath());
                            body.append(QString("Content-Disposition: form-data; name=\"%1\"; filename=\"%2\"\r\n").arg(key).arg(info.fileName()).toUtf8());
                            body.append("Content-Type: image/jpeg\r\n");
//                            body.append("Content-Transfer-Encoding: binary\r\n");
                            body.append("\r\n");
                            if (file.open(QFile::ReadOnly)) {
                                body.append(file.readAll());
                                file.close();
                            } else {
                                qDebug() << Q_FUNC_INFO << __LINE__ << file.errorString();
                            }
                            body.append("\r\n");
                        } else {
                            body.append(QString("Content-Disposition: form-data; name=\"%1\"\r\n").arg(key).toUtf8());
                            body.append("\r\n");
                            body.append(val);
                            body.append("\r\n");
                        }
                    }
                }
                body.append(QString("\r\n--%1--\r\n").arg(boundary).toUtf8());
                request.setHeader(QNetworkRequest::ContentLengthHeader, QString::number(body.length()).toUtf8());
                request.setHeader(QNetworkRequest::ContentTypeHeader, QString("multipart/form-data; boundary=%1").arg(boundary).toUtf8());

                ret = q->networkAccessManager()->post(request, body);
            } else {
                request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/x-www-form-urlencoded"));
                request.setRawHeader("Authorization", authHeader(params));
                request.setUrl(url);
                ret = q->networkAccessManager()->post(request, normalize(QMultiMap<QString, QByteArray>()));
            }
            break;
        case AuthorizeByBody: {
            QUrl qurl(url);
            QUrlQuery query;
            QList<QString> keys = requestParams.keys();
            foreach(const QString &key, keys) {
                QList<QByteArray> vals = requestParams.values(key);
                foreach(const QByteArray &val, vals) {
                    query.addQueryItem(key, QString::fromUtf8(val));
//                    qurl.addQueryItem(parameterEncoding(key), val.toPercentEncoding());
                }
            }
            qurl.setQuery(query);
            request.setUrl(qurl);
            ret = q->networkAccessManager()->post(request, normalize(params));
            break;
        }
        case AuthorizeByUrl: {
            QMultiMap<QString, QByteArray> allParams(requestParams);
            allParams += params;
            QUrl qurl(url);
            QUrlQuery query;
            QList<QString> keys = allParams.keys();
            foreach(const QString &key, keys) {
                QList<QByteArray> vals = allParams.values(key);
                foreach(const QByteArray &val, vals) {
                    query.addQueryItem(key, val);
//                    qurl.addQueryItem(parameterEncoding(key), val.toPercentEncoding());
                }
            }
            qurl.setQuery(query);
            request.setUrl(qurl);
            ret = q->networkAccessManager()->post(request, QByteArray());
            break;
        }
        }

    } else if (method == "GET") {
        switch (authorizeBy) {
        case AuthorizeByHeader:
            request.setRawHeader("Authorization", authHeader(params));
            request.setUrl(url);
            ret = q->networkAccessManager()->get(request);
            break;
        case AuthorizeByBody:
            qWarning() << "GET doesn't support AuthorizeByBody.";
            break;
        case AuthorizeByUrl: {
            QMultiMap<QString, QByteArray> allParams(requestParams);
            allParams += params;
            QUrl qurl(url);
            QUrlQuery query;
            QList<QString> keys = allParams.keys();
            foreach(const QString &key, keys) {
                QList<QByteArray> vals = allParams.values(key);
                foreach(const QByteArray &val, vals) {
                    query.addQueryItem(key, QString::fromUtf8(val));
//                    qurl.addEncodedQueryItem(parameterEncoding(key), val.toPercentEncoding());
                }
            }
            qurl.setQuery(query);
            request.setUrl(qurl);
            ret = q->networkAccessManager()->get(request);
            break;
        }
        }
    }
//    qDebug() << Q_FUNC_INFO << __LINE__ << ret->url();
    return ret;
}

QString OAuth::Private::sign(const QString &method, const QUrl &url, const QVariantMap &params)
{
//    qDebug() << Q_FUNC_INFO << __LINE__ << method << url << params << multiPart;
    QMultiMap<QString, QByteArray> params2;
    foreach (const QString &key, params.keys()) {
        params2.insert(key, params.value(key).toString().toUtf8());
    }

    QDateTime dt = QDateTime::currentDateTimeUtc();
    QString timestamp;
    QString nonce;
    timestamp.setNum(dt.toTime_t());
    nonce.setNum(dt.toMSecsSinceEpoch());

    if (requestParams.isEmpty()) {
        requestParams["oauth_version"] = "1.0";
        requestParams["oauth_signature_method"] = "HMAC-SHA1";
        requestParams["oauth_consumer_key"] = q->m_consumerKey.toUtf8();
        if (!q->m_token.isEmpty())
            requestParams["oauth_token"] = q->m_token.toUtf8();
        if (!q->m_callbackUrl.isEmpty())
            requestParams["oauth_callback"] = q->m_callbackUrl.toString().toUtf8();
    }
    requestParams["oauth_nonce"] = nonce.toUtf8();
    requestParams["oauth_timestamp"] = timestamp.toUtf8();
    requestParams["oauth_signature"] =  signature(method, url, normalize(signatureParams(params2)));

//    qDebug() << Q_FUNC_INFO << __LINE__ << requestParams;

    QMultiMap<QString, QByteArray> allParams(requestParams);
    allParams += params2;

    QUrl qurl(url);
    QUrlQuery query;
    QList<QString> keys = allParams.keys();
    foreach(const QString &key, keys) {
        QList<QByteArray> vals = allParams.values(key);
        foreach(const QByteArray &val, vals) {
            query.addQueryItem(key, val);
        }
    }
    qurl.setQuery(query);

//    qDebug() << Q_FUNC_INFO << __LINE__ << ret->url();
    return qurl.toString();
}

QString OAuth::Private::authHeader(const QString &method, const QUrl &url, const QVariantMap &params)
{
//    qDebug() << Q_FUNC_INFO << __LINE__ << method << url << params << multiPart;

    QDateTime dt = QDateTime::currentDateTimeUtc();
    QString timestamp;
    QString nonce;
    timestamp.setNum(dt.toTime_t());
    nonce.setNum(dt.toMSecsSinceEpoch());

    if (requestParams.isEmpty()) {
        requestParams["oauth_version"] = "1.0";
        requestParams["oauth_signature_method"] = "HMAC-SHA1";
        requestParams["oauth_consumer_key"] = q->m_consumerKey.toUtf8();
        if (!q->m_token.isEmpty())
            requestParams["oauth_token"] = q->m_token.toUtf8();
        if (!q->m_callbackUrl.isEmpty())
            requestParams["oauth_callback"] = q->m_callbackUrl.toString().toUtf8();
    }
    requestParams["oauth_nonce"] = nonce.toUtf8();
    requestParams["oauth_timestamp"] = timestamp.toUtf8();
    qDebug() << Q_FUNC_INFO << __LINE__ << requestParams;
    QMultiMap<QString, QByteArray> params2;
    foreach (const QString &key, params.keys()) {
        params2.insert(key, params.value(key).toString().toUtf8());
    }

    requestParams["oauth_signature"] =  signature(method, url, normalize(signatureParams(params2)));

    return authHeader(params2);
}

QByteArray OAuth::Private::parameterEncoding(const QString &str) const
{
    return str.toUtf8().toPercentEncoding();
}

QByteArray OAuth::Private::normalize(const QMultiMap<QString, QByteArray> &param) const
{
    QByteArray ret;
    QList<QString> keys = param.keys();
    foreach(const QString &key, keys) {
        QList<QByteArray> vals = param.values(key);
        foreach(const QByteArray &val, vals) {
            ret += parameterEncoding(key) + '=' + val.toPercentEncoding() + '&';
        }
    }
    if (keys.size() > 0)
        ret.chop(1);
    return ret;
}

QByteArray OAuth::Private::signature(const QString &method, const QUrl &url, const QByteArray &params) const
{
    QByteArray key = parameterEncoding(q->m_consumerSecret) + "&" +
                     parameterEncoding(q->m_tokenSecret);
    QByteArray base = parameterEncoding(method) + "&" +
                      parameterEncoding(url.toString()) + "&" +
                      params.toPercentEncoding();
    return hmac_sha1(key,base).toBase64();
}

QMultiMap<QString, QByteArray> OAuth::Private::signatureParams(const QMultiMap<QString, QByteArray> &params) const
{
    QMultiMap<QString, QByteArray> ret(requestParams);
    ret.remove("oauth_signature");
    ret += params;
    return ret;
}

QByteArray OAuth::Private::authHeader(const QMultiMap<QString, QByteArray> &params, const QUrl &realm) const
{
    QByteArray ret = "OAuth ";
    if (realm.isValid()) {
        ret += "realm=\"" + realm.toString() + "\",";
    }
    QMultiMap<QString, QByteArray> map(requestParams);
    map += params;
    QMapIterator<QString, QByteArray> i(map);
    while (i.hasNext()) {
        i.next();
        ret += parameterEncoding(i.key());
        ret += "=";
        ret += '"';
        ret += i.value().toPercentEncoding();
        ret += '"';
        if (i.hasNext()) {
            ret += ",";
        }
    }
    return ret;
}

bool OAuth::Private::updateToken(QNetworkReply* reply)
{
    if (!reply) {
        return false;
    }
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "error : " << reply->errorString();
        return false;
    }
    QRegExp exp("(([^=&?]+)=([^=&?]+))(&([^=&?]+)=([^=&?]+))*");
    QByteArray result = reply->readAll();
    if (!exp.exactMatch(result)) {
        return false;
    }
    QList<QByteArray> params = result.split('&');
    foreach (const QByteArray &param, params) {
        QList<QByteArray> tmp = param.split('=');
        QByteArray key = QByteArray::fromPercentEncoding(tmp.at(0));
        QByteArray val = QByteArray::fromPercentEncoding(tmp.at(1));
        if (key == "oauth_token_secret") {
            q->tokenSecret(val);
        } else if (key == "oauth_token") {
                q->token(val);
        } else {
            const QMetaObject *mo = q->metaObject();
            bool found = false;
            for (int i = 0; i < mo->propertyCount(); i++) {
                QMetaProperty p = mo->property(i);
                if (key == p.name()) {
                    p.write(q, QString(val));
                    found = true;
                    break;
                }
            }
            if (!found)
                requestParams[key] = val;
        }
    }
    return true;
}

OAuth::OAuth(QObject *parent)
    : SilkAbstractObject(parent)
    , m_authorized(false)
{
    d = new Private(this);
}

void OAuth::requestToken()
{
    d->requestToken();
}

void OAuth::accessToken(const QString &verifier)
{
    d->accessToken(verifier);
}

QNetworkAccessManager *OAuth::networkAccessManager() const
{
    if (!d->networkAccessManager)
        d->networkAccessManager = new QNetworkAccessManager(d);
    return d->networkAccessManager;
}

void OAuth::setNetworkAccessManager(QNetworkAccessManager *networkAccessManager)
{
    if (d->networkAccessManager == networkAccessManager) return;
    d->networkAccessManager = networkAccessManager;
    emit networkAccessManagerChanged(networkAccessManager);
}

OAuth::AuthorizeBy OAuth::authorizeBy() const
{
    return d->authorizeBy;
}

void OAuth::setAuthorizeBy(AuthorizeBy authorizeBy)
{
    d->authorizeBy = authorizeBy;
}

QString OAuth::sign(const QString &method, const QUrl &url, const QVariantMap &params)
{
    return d->sign(method, url, params);
}

QString OAuth::authHeader(const QString &method, const QUrl &url, const QVariantMap &params)
{
    return d->authHeader(method, url, params);
}

#include "oauth.moc"

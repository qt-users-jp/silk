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

#include "httphandler.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkCookie>

#include <qhttprequest.h>
#include <qhttpreply.h>

class HttpHandler::Private : public QObject
{
    Q_OBJECT
public:
    Private(HttpHandler *parent);

    void load(const QUrl &url, QHttpRequest *request, QHttpReply *reply, const QString &message);

private slots:
    void finished();
    void error(QNetworkReply::NetworkError error);
    void httpReplyDestroyed(QObject *object);

private:
    HttpHandler *q;
    QMap<QObject *, QHttpRequest*> requestMap;
    QMap<QObject *, QHttpReply*> replyMap;
    QMap<QObject *, QNetworkReply*> replyMap2;
    static QNetworkAccessManager networkAccessManager;
};

QNetworkAccessManager HttpHandler::Private::networkAccessManager;

HttpHandler::Private::Private(HttpHandler *parent)
    : QObject(parent)
    , q(parent)
{
}

void HttpHandler::Private::load(const QUrl &url, QHttpRequest *request, QHttpReply *reply, const QString &message)
{
//    qDebug() << url;
    Q_UNUSED(message)
    QNetworkRequest req(url);

    foreach (const QByteArray &headerName, request->rawHeaderList()) {
        req.setRawHeader(headerName, request->rawHeader(headerName));
    }

    QNetworkReply *rep;
    if (request->method() == "POST") {
        rep = networkAccessManager.post(req, request);
    } else if (request->method() == "PUT") {
        rep = networkAccessManager.put(req, request);
    } else if (request->method() == "GET") {
        rep = networkAccessManager.get(req);
    } else if (request->method() == "HEAD") {
        rep = networkAccessManager.head(req);
    } else {
        rep = networkAccessManager.sendCustomRequest(req, request->method(), request);
    }
    requestMap.insert(rep, request);
    replyMap.insert(rep, reply);
    replyMap2.insert(reply, rep);
    connect(rep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    connect(rep, SIGNAL(finished()), this, SLOT(finished()));
    connect(reply, SIGNAL(destroyed(QObject*)), this, SLOT(httpReplyDestroyed(QObject*)));
}

void HttpHandler::Private::finished()
{
    QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());
//    qDebug() << Q_FUNC_INFO << __LINE__ << rep->url();
    /*QHttpRequest *request = */requestMap.take(rep);
    QHttpReply *reply = replyMap.take(rep);
    replyMap2.take(reply);
    foreach (const QByteArray &headerName, rep->rawHeaderList()) {
        reply->setRawHeader(headerName, rep->rawHeader(headerName));
//        qDebug() << headerName << rep->rawHeader(headerName);
    }
    int status = rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//    qDebug() << status;
    reply->setStatus(status);
    QByteArray data = rep->readAll();
//    qDebug() << data.length();
    reply->write(data);
    reply->close();
    rep->deleteLater();
//    qDebug() << Q_FUNC_INFO << __LINE__;
}

void HttpHandler::Private::error(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)
    QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());
//    qDebug() << Q_FUNC_INFO << __LINE__ << rep->url() << error;
    QHttpRequest *request = requestMap.value(rep);
    QHttpReply *reply = replyMap.value(rep);
    emit q->error(403, request, reply, rep->errorString());
//    qDebug() << Q_FUNC_INFO << __LINE__;
}

void HttpHandler::Private::httpReplyDestroyed(QObject* object)
{
//    qDebug() << Q_FUNC_INFO << __LINE__;
    if (replyMap2.contains(object)) {
        QNetworkReply *reply = replyMap2.take(object);
        reply->deleteLater();
    }
//    qDebug() << Q_FUNC_INFO << __LINE__;
}

HttpHandler::HttpHandler(QObject *parent)
    : SilkAbstractProtocolHandler(parent)
    , d(new Private(this))
{
}

bool HttpHandler::load(const QUrl &url, QHttpRequest *request, QHttpReply *reply, const QString &message)
{
    d->load(url, request, reply, message);
    return true;
}

#include "httphandler.moc"

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

#include "sssohandler.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkCookie>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include <qhttprequest.h>
#include <qhttpreply.h>

#include <silkconfig.h>

class SSSOHandler::Private : public QObject
{
    Q_OBJECT
public:
    Private(SSSOHandler *parent);

    bool loggedIn(QHttpRequest *request, QString *username, QString *password);
    void redirect(QHttpRequest *request, QHttpReply *reply);
    void load(const QUrl &url, QHttpRequest *request, QHttpReply *reply, const QString &username, const QString &password);

private:

private slots:
    void finished();
    void error(QNetworkReply::NetworkError error);
    void httpReplyDestroyed(QObject *object);

private:
    SSSOHandler *q;
    QMap<QObject *, QHttpRequest*> requestMap;
    QMap<QObject *, QHttpReply*> replyMap;
    QMap<QObject *, QNetworkReply*> replyMap2;
    static QNetworkAccessManager networkAccessManager;
};

QNetworkAccessManager SSSOHandler::Private::networkAccessManager;

SSSOHandler::Private::Private(SSSOHandler *parent)
    : QObject(parent)
    , q(parent)
{
}

void SSSOHandler::Private::redirect(QHttpRequest *request, QHttpReply *reply)
{
    reply->setStatus(302);
    QNetworkCookie cookie;
    cookie.setDomain(SilkConfig::value("ssso.domain").toString());
    cookie.setPath("/");
    cookie.setName("ssso_from");
    cookie.setValue(request->url().toString().toUtf8());
    reply->setCookies(QList<QNetworkCookie>() << cookie);

    reply->setRawHeader("Location", SilkConfig::value("ssso.loginUrl").toString().toUtf8());
    reply->close();
}

bool SSSOHandler::Private::loggedIn(QHttpRequest *request, QString *username, QString *password)
{
    QString sssoSessionId;

    foreach (const QNetworkCookie &cookie, request->cookies()) {
        if (cookie.name() == "ssso_session_id") {
            sssoSessionId = cookie.value();
            break;
        }
    }

//    qDebug() << Q_FUNC_INFO << __LINE__ << sssoSessionId;

    if (sssoSessionId.isNull()) return false;

    QString connectionName = SilkConfig::value("ssso.database.connectionName").toString();
    QSqlDatabase db;
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        QString type = SilkConfig::value("ssso.database.type").toString();
        db = QSqlDatabase::addDatabase(type, connectionName);
        db.setHostName(SilkConfig::value("ssso.database.hostName").toString());
        db.setDatabaseName(SilkConfig::value("ssso.database.databaseName").toString());
        db.setUserName(SilkConfig::value("ssso.database.userName").toString());
        db.setPassword(SilkConfig::value("ssso.database.password").toString());
        if (!db.open()) {
            qDebug() << Q_FUNC_INFO << __LINE__ << db.lastError();
            return false;
        }
    }

    if (!db.tables().contains("session")) return false;

    QString sql = QString("SELECT account.username, account.password FROM account INNER JOIN session ON account.id = session.account_id WHERE session.key = '%1'").arg(sssoSessionId);
    QSqlQuery query(sql, db);
    if (!query.first()) {
        qDebug() << Q_FUNC_INFO << __LINE__ << sql << query.lastError();
        return false;
    }

    *username = query.value(0).toString();
    *password = query.value(1).toString();
//    qDebug() << Q_FUNC_INFO << __LINE__ << *username << *password;
    return true;
}

void SSSOHandler::Private::load(const QUrl &url, QHttpRequest *request, QHttpReply *reply, const QString &username, const QString &password)
{
//    qDebug() << Q_FUNC_INFO << __LINE__ << url << username;
    QUrl http = url;
    http.setScheme("http");
    QNetworkRequest req(http);

    foreach (const QByteArray &headerName, request->rawHeaderList()) {
        req.setRawHeader(headerName, request->rawHeader(headerName));
    }

    req.setRawHeader("X-Forwarded-User", username.toUtf8());

    // TODO: cookie

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

void SSSOHandler::Private::finished()
{
    QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());
    QHttpRequest *request = requestMap.take(rep);
    QHttpReply *reply = replyMap.take(rep);
    replyMap2.take(reply);
    foreach (const QByteArray &headerName, rep->rawHeaderList()) {
        QByteArray value = rep->rawHeader(headerName);
        if (rep->url().port() == 80) {
            value.replace(QString("://%1/").arg(rep->url().host()), QString("://%1/").arg(request->url().host()).toUtf8());
        } else {
            value.replace(QString("://%1:%2/").arg(rep->url().host()).arg(rep->url().port()), QString("://%1:%2/").arg(request->url().host()).arg(request->url().port()).toUtf8());
        }
//        qDebug() << Q_FUNC_INFO << __LINE__ << headerName << value;
        reply->setRawHeader(headerName, value);
    }
    int status = rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//    qDebug() << Q_FUNC_INFO << __LINE__ << status << request->url() << rep->url();
    reply->setStatus(status);
    QByteArray data = rep->readAll();
    reply->write(data);
    reply->close();
    rep->deleteLater();
}

void SSSOHandler::Private::error(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)
    QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());
//    qDebug() << Q_FUNC_INFO << __LINE__ << rep->url() << error;
    QHttpRequest *request = requestMap.value(rep);
    QHttpReply *reply = replyMap.value(rep);
    emit q->error(403, request, reply, rep->errorString());
//    qDebug() << Q_FUNC_INFO << __LINE__;
}

void SSSOHandler::Private::httpReplyDestroyed(QObject* object)
{
//    qDebug() << Q_FUNC_INFO << __LINE__;
    if (replyMap2.contains(object)) {
        QNetworkReply *reply = replyMap2.take(object);
        reply->deleteLater();
    }
//    qDebug() << Q_FUNC_INFO << __LINE__;
}

SSSOHandler::SSSOHandler(QObject *parent)
    : SilkAbstractProtocolHandler(parent)
    , d(new Private(this))
{
}

bool SSSOHandler::load(const QUrl &url, QHttpRequest *request, QHttpReply *reply, const QString &message)
{
    Q_UNUSED(message)
//    qDebug() << Q_FUNC_INFO << __LINE__ << url;
    QString username;
    QString password;
    if (!d->loggedIn(request, &username, &password)) {
//        qDebug() << Q_FUNC_INFO << __LINE__;
        d->redirect(request, reply);
    } else {
//        qDebug() << Q_FUNC_INFO << __LINE__ << username << password;
        d->load(url, request, reply, username, password);
    }
    return true;
}

#include "sssohandler.moc"

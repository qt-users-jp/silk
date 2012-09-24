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

#include "silk.h"
#include "silkconfig.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QMimeDatabase>
#include <QtCore/QPluginLoader>
#include <QtNetwork/QNetworkCookie>
#include <QtQml/qqml.h>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>

#include <qhttprequest.h>
#include <qhttpreply.h>

#include <silkimportsinterface.h>

#include "silkhttpobject.h"

class Silk::Private : public QObject
{
    Q_OBJECT
public:
    Private(Silk *parent);

private slots:
    void incomingConnection(QHttpRequest *request, QHttpReply *reply);
    void statusChanged();
    void loadingChanged(bool loading);
    void componentDestroyed(QObject *object);
    void clearQmlCache();

private:
    QString documentRootForRequest(const QHttpRequest *request) const;
    void loadQml(const QFileInfo &fileInfo, QHttpRequest *request, QHttpReply *reply, const QString &message = QString());
    void execQml(QQmlComponent *component, QHttpRequest *request, QHttpReply *reply, const QString &message = QString());
    void loadFile(const QFileInfo &fileInfo, QHttpRequest *request, QHttpReply *reply);
    void error(int statusCode, QHttpRequest *request, QHttpReply *reply, const QString &message = QString());
    void close(SilkHttpObject *http);

private:
    Silk *q;
    QMimeDatabase mimeDatabase;
    QQmlEngine engine;
    QMap<QObject*, QString> component2root;
    QMap<QObject*, QHttpRequest*> component2request;
    QMap<QObject*, QHttpReply*> component2reply;
    QMap<QObject*, QString> component2message;
    QMap<QObject*, SilkHttpObject*> component2http;
    QMap<QObject*, QHttpRequest*> http2request;
    QMap<QObject*, QHttpReply*> http2reply;

public:
    QMap<QString, QString> documentRoots;
};

Silk::Private::Private(Silk *parent)
    : QObject(parent)
    , q(parent)
{
    qmlRegisterType<SilkAbstractHttpObject>();
    qmlRegisterType<SilkHttpObject>("Silk.HTTP", 1, 1, "Http");

    QDir appDir = QCoreApplication::applicationDirPath();
    QDir importsDir = appDir;
    QString appPath(SILK_APP_PATH);
    // up to system root path
    for (int i = 0; i < appPath.count(QLatin1Char('/')) + 1; i++) {
        importsDir.cdUp();
    }
    importsDir.cd(SILK_IMPORTS_PATH);
    foreach (const QString &lib, importsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(importsDir.absoluteFilePath(lib));
        if (pluginLoader.load()) {
            QObject *object = pluginLoader.instance();
            if (object) {
                SilkImportsInterface *plugin = qobject_cast<SilkImportsInterface *>(object);
                if (plugin) {
                    plugin->silkRegisterObject();
                } else {
                    qWarning() << object;
                }
            } else {
                qWarning() << Q_FUNC_INFO << __LINE__;
            }
        } else {
            qWarning() << pluginLoader.errorString() << importsDir.absoluteFilePath(lib);
        }
    }

    engine.setOfflineStoragePath(appDir.absoluteFilePath(SilkConfig::value("storage").toMap().value("path").toString()));
    engine.addImportPath(":/imports");
    foreach (const QString &importPath, SilkConfig::value("import").toMap().value("path").toStringList()) {
        engine.addImportPath(appDir.absoluteFilePath(importPath));
    }
    connect(q, SIGNAL(incomingConnection(QHttpRequest *, QHttpReply *)), this, SLOT(incomingConnection(QHttpRequest *, QHttpReply *)));

    QString listenAddress = SilkConfig::value("listen").toMap().value("address").toString();
    QHostAddress address;
    if (listenAddress == QLatin1String("*")) {
        address = QHostAddress::Any;
    } else if (listenAddress == QLatin1String("localhost")) {
        address = QHostAddress::LocalHost;
    } else if (!address.setAddress(listenAddress)) {
        qWarning() << "The address" << listenAddress << "is not available.";
        QMetaObject::invokeMethod(QCoreApplication::instance(), "quit", Qt::QueuedConnection);
        return;
    }

    int port = SilkConfig::value("listen").toMap().value("port").toInt();

    QVariantMap roots = SilkConfig::value("contents").toMap();
    foreach (const QString &key, roots.keys()) {
        documentRoots.insert(key, appDir.absoluteFilePath(roots.value(key).toString()));
    }

    if (!q->listen(address, port)) {
        qWarning() << q->errorString();
        QMetaObject::invokeMethod(QCoreApplication::instance(), "quit", Qt::QueuedConnection);
    }
}

QString Silk::Private::documentRootForRequest(const QHttpRequest *request) const
{
    QString ret(":/contents");
    if (documentRoots.contains(request->rawHeader("Host"))) {
        ret = documentRoots.value(request->rawHeader("Host"));
    } else if (documentRoots.contains("*")) {
        ret = documentRoots.value("*");
    }
    return ret;
}

void Silk::Private::incomingConnection(QHttpRequest *request, QHttpReply *reply)
{
//    qDebug() << request->url();

    QString documentRoot = documentRootForRequest(request);

    QString fileName(documentRoot + request->url().path());
    QFileInfo fileInfo(fileName);
    if (fileInfo.isDir()) {
        if (request->url().path().endsWith("/")) {
            fileName = fileName + QLatin1String("/index.qml");
            fileInfo = QFileInfo(fileName);
        } else {
            QUrl url(request->url());
            url.setPath(url.path() + "/");
            error(301, request, reply, url.toString());
            return;
        }
    }

    if (fileInfo.exists()) {
        QMimeType mimeType = mimeDatabase.mimeTypeForFile(fileInfo.fileName(), QMimeDatabase::MatchExtension);
        reply->setStatus(200);
        reply->setRawHeader("Content-Type", mimeType.name().toUtf8());
        if (mimeType.name() == "text/x-qml") {
            if (fileInfo.fileName().at(0).isUpper()) {
                loadFile(fileInfo, request, reply);
            } else if (!fileInfo.isReadable()){
                error(403, request, reply, request->url().toString());
            } else {
                loadQml(fileInfo, request, reply);
            }
        } else {
            loadFile(fileInfo, request, reply);
        }
    } else {
        error(404, request, reply, request->url().toString());
    }
}

void Silk::Private::loadQml(const QFileInfo &fileInfo, QHttpRequest *request, QHttpReply *reply, const QString &message)
{
    QUrl url;
    if (fileInfo.path().startsWith(':')) {
        url = QUrl(QString("qrc%1").arg(fileInfo.dir().path()));
    } else {
        url = QUrl::fromLocalFile(fileInfo.absoluteDir().path());
    }
    url.setPath(url.path() + "/" + fileInfo.fileName());
    QQmlComponent *component = new QQmlComponent(&engine, url, reply);
    connect(component, SIGNAL(destroyed(QObject *)), this, SLOT(componentDestroyed(QObject *)), Qt::QueuedConnection);
    execQml(component, request, reply, message);
}

void Silk::Private::execQml(QQmlComponent *component, QHttpRequest *request, QHttpReply *reply, const QString &message)
{
    static bool cache = SilkConfig::value("cache").toMap().value("qml").toBool();
    switch (component->status()) {
    case QQmlComponent::Null:
        // TODO: any check?
        break;
    case QQmlComponent::Error:
        qDebug() << component->errorString();
        error(500, request, reply, component->errorString());
        break;
    case QQmlComponent::Loading:
        component2request.insert(component, request);
        component2reply.insert(component, reply);
        component2message.insert(component, message);
        connect(component, SIGNAL(statusChanged(QQmlComponent::Status)), this, SLOT(statusChanged()), Qt::UniqueConnection);
        break;
    case QQmlComponent::Ready: {
        SilkHttpObject *http = qobject_cast<SilkHttpObject *>(component->create());
        if (!cache)
            connect(http, SIGNAL(destroyed()), this, SLOT(clearQmlCache()), Qt::QueuedConnection);
        http->method(QString::fromLatin1(request->method()));
        QUrl url(request->url());
        QString query(url.query());
        url.setQuery(QString());
        http->scheme(url.scheme());
        http->host(url.host());
        http->path(url.path());
        http->query(query);
        http->data(QString(request->readAll()));

        QVariantMap requestHeader;
        foreach (const QByteArray &key, request->rawHeaderList()) {
            requestHeader.insert(QString(key), QString(request->rawHeader(key)));
        }
        http->requestHeader(requestHeader);

        QVariantMap cookies;
        foreach (const QNetworkCookie &cookie, request->cookies()) {
            QVariantMap c;
            c.insert(QLatin1String("value"), QString::fromUtf8(cookie.value()));
            c.insert(QLatin1String("expires"), cookie.expirationDate());
            c.insert(QLatin1String("domain"), cookie.domain());
            c.insert(QLatin1String("path"), cookie.path());
            c.insert(QLatin1String("secure"), cookie.isSecure());
            c.insert(QLatin1String("session"), cookie.isSessionCookie());
            cookies.insert(QString::fromUtf8(cookie.name()), c);
        }
        http->requestCookies(cookies);

        if (!message.isEmpty()) http->message(message);
        QMetaObject::invokeMethod(http, "ready");

        component2http.insert(component, http);
        http2request.insert(http, request);
        http2reply.insert(http, reply);
        if (!http->loading()) {
            close(http);
        } else {
            connect(http, SIGNAL(loadingChanged(bool)), this, SLOT(loadingChanged(bool)));
        }
        break; }
    }
}

void Silk::Private::componentDestroyed(QObject *object)
{
    if (component2request.contains(object)) {
        component2request.remove(object);
    }
    if (component2reply.contains(object)) {
        component2reply.remove(object);
    }
    if (component2message.contains(object)) {
        component2message.remove(object);
    }
    if (component2http.contains(object)) {
        SilkHttpObject *http = component2http.take(object);
        http2request.remove(http);
        http2reply.remove(http);
    }
}

void Silk::Private::statusChanged()
{
    QQmlComponent *component = qobject_cast<QQmlComponent *>(sender());
    execQml(component, component2request.take(component), component2reply.take(component), component2message.take(component));
}

void Silk::Private::loadingChanged(bool loading)
{
    if (!loading) {
        SilkHttpObject *http = qobject_cast<SilkHttpObject *>(sender());
        close(http);
    }
}

void Silk::Private::close(SilkHttpObject *http)
{
    if (http2request.contains(http) && http2reply.contains(http)) {
        QHttpRequest *request = http2request.take(http);
        QHttpReply *reply = http2reply.take(http);
        reply->setStatus(http->status());

        QVariantMap header = http->responseHeader();
        foreach (const QString &key, header.keys()) {
            QString value = header.value(key).toString();
            reply->setRawHeader(key.toUtf8(), value.toUtf8());
        }

        QList<QNetworkCookie> cookies;
        foreach (const QString &name, http->responseCookies().keys()) {
            QVariantMap c = http->responseCookies().value(name).toMap();
            QNetworkCookie cookie;
            cookie.setName(name.toUtf8());
            if (c.contains("value")) cookie.setValue(c.value("value").toString().toUtf8());
            if (c.contains("expires")) cookie.setExpirationDate(c.value("expires").toDateTime());
            if (c.contains("domain")) cookie.setDomain(c.value("domain").toString());
            if (c.contains("path")) cookie.setPath(c.value("path").toString());
            if (c.contains("secure")) cookie.setSecure(c.value("secure").toBool());
            cookies.append(cookie);
        }
        reply->setCookies(cookies);

        if (request->method() == "GET" || request->method() == "POST") {
            reply->write(http->out());
        }
        reply->close();
    }
    http->deleteLater();
}

void Silk::Private::clearQmlCache()
{
    engine.trimComponentCache();
}

void Silk::Private::loadFile(const QFileInfo &fileInfo, QHttpRequest *request, QHttpReply *reply)
{
    if (fileInfo.fileName().startsWith(".")) {
        error(403, request, reply, request->url().toString());
    } else {
        // TODO cache
        QFile file(fileInfo.absoluteFilePath());
        if (file.open(QFile::ReadOnly)) {
            QDateTime lastModified = fileInfo.lastModified().toUTC();
            reply->setRawHeader("Last-Modified", lastModified.toString("ddd, d, MMM yyyy hh:mm:ss UTC").toUtf8());
            reply->write(file.readAll());
            file.close();
            reply->close();
        } else {
            error(403, request, reply, request->url().toString());
        }
    }
}

void Silk::Private::error(int statusCode, QHttpRequest *request, QHttpReply *reply, const QString &message)
{
    QString documentRoot = documentRootForRequest(request);
    if (QFile::exists(QString::fromUtf8("%1/errors/%2.qml").arg(documentRoot).arg(statusCode))) {
        loadQml(QFileInfo(QString::fromUtf8("%1/errors/%2.qml").arg(documentRoot).arg(statusCode)), request, reply, message);
    } else {
        loadQml(QFileInfo(QString::fromUtf8(":/errors/%2.qml").arg(statusCode)), request, reply, message);
    }
}

Silk::Silk(QObject *parent)
    : QHttpServer(parent)
    , d(new Private(this))
{
}

const QMap<QString, QString> &Silk::documentRoots() const
{
    return d->documentRoots;
}

void Silk::setDocumentRoots(const QMap<QString, QString> &documentRoots)
{
    if (d->documentRoots == documentRoots) return;
    d->documentRoots = documentRoots;
    emit documentRootsChanged(documentRoots);
}

#include "silk.moc"

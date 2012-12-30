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

#include "qmlhandler.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkCookie>
#include <QtQml/qqml.h>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>

#include <qhttprequest.h>
#include <qhttpreply.h>
#include <qwebsocket.h>

#include <silkconfig.h>
#include <silkimportsinterface.h>

#include "text.h"
#include "httpobject.h"
#include "websocketobject.h"
#include "silk.h"

class QmlHandler::Private : public QObject
{
    Q_OBJECT
public:
    Private(QmlHandler *parent);

    void load(const QUrl &url, QHttpRequest *request, QHttpReply *reply, const QString &message);
    void load(const QUrl &url, QWebSocket *socket, const QString &message);
private:
    void exec(QQmlComponent *component, QHttpRequest *request, QHttpReply *reply, const QString &message = QString());
    void exec(QQmlComponent *component, QWebSocket *socket, const QString &message = QString());
    void close(SilkAbstractHttpObject *http);

private slots:
    void loadingChanged(bool loading);
    void statusChanged();
    void componentDestroyed(QObject *object);
    void clearQmlCache();
    void registerObject(const char *uri, int major, int minor);

private:
    QmlHandler *q;
    QQmlEngine engine;
    QMap<QObject*, QString> component2root;
    QMap<QObject*, QHttpRequest*> component2request;
    QMap<QObject*, QHttpReply*> component2reply;
    QMap<QObject*, QWebSocket*> component2socket;
    QMap<QObject*, QString> component2message;
    QMap<QObject*, QObject*> component2object;
    QMap<QObject*, QHttpRequest*> object2request;
    QMap<QObject*, QHttpReply*> object2reply;
    QMap<QObject*, QQmlContext*> object2context;
    QMap<QObject*, HttpObject*> object2http;
};

QmlHandler::Private::Private(QmlHandler *parent)
    : QObject(parent)
    , q(parent)
{
    QQmlContext *context = engine.rootContext();
    context->setContextProperty(QLatin1String("Silk"), new Silk(this));

    qmlRegisterType<SilkAbstractHttpObject>();
    qmlRegisterUncreatableType<HttpFileData>("Silk.HTTP", 1, 1, "HttpFileData", QLatin1String("readonly"));
    qmlRegisterType<WebSocketObject>("Silk.WebSocket", 1, 0, "WebSocket");

    QDir appDir = QCoreApplication::applicationDirPath();
    QDir importsDir = appDir;
    QString appPath(SILK_APP_PATH);
    // up to system root path
    for (int i = 0; i < appPath.count(QLatin1Char('/')) + 1; i++) {
        importsDir.cdUp();
    }
    importsDir.cd(SILK_IMPORTS_PATH);

    QMap<QString, QObject*> plugins;
    foreach (const QString &lib, importsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(importsDir.absoluteFilePath(lib));
        if (pluginLoader.load()) {
            QObject *object = pluginLoader.instance();
            if (object) {
                SilkImportsInterface *plugin = qobject_cast<SilkImportsInterface *>(object);
                if (plugin) {
                    plugins.insert(plugin->name(), object);
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

    foreach (QObject *plugin, plugins.values()) {
        connect(plugin, SIGNAL(registerObject(const char*,int,int)), this, SLOT(registerObject(const char*,int,int)));
        foreach (const QString &parent, qobject_cast<SilkImportsInterface *>(plugin)->parents()) {
            if (plugins.contains(parent)) {
                connect(plugin, SIGNAL(registerObject(const char*,int,int)), plugins.value(parent), SLOT(silkRegisterObject(const char*,int,int)));
            }
        }
    }
    foreach (QObject *plugin, plugins.values()) {
        qobject_cast<SilkImportsInterface *>(plugin)->silkRegisterObject();
    }

    engine.setOfflineStoragePath(appDir.absoluteFilePath(SilkConfig::value("storage.path").toString()));
    engine.addImportPath(":/imports");
    foreach (const QString &importPath, SilkConfig::value("import.path").toStringList()) {
        engine.addImportPath(appDir.absoluteFilePath(importPath));
    }

    QVariantList tasks = SilkConfig::value("silk.tasks").toList();
    foreach (const QVariant &task, tasks) {
        QUrl url;
        if (task.toString().startsWith(":")) {
            url = QUrl(QLatin1String("qrc") + task.toString());
        } else {
            url = QUrl::fromLocalFile(task.toString());
        }
        QQmlComponent *component = new QQmlComponent(&engine, url, this);
        switch (component->status()) {
        case QQmlComponent::Null:
            break;
        case QQmlComponent::Error:
            qDebug() << component->errorString();
            QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
            break;
        case QQmlComponent::Loading:
            break;
        case QQmlComponent::Ready: {
            QObject *app = component->create();
            connect(app, SIGNAL(destroyed(QObject *)), qApp, SLOT(quit()), Qt::QueuedConnection);
            break; }
        }
    }
}

void QmlHandler::Private::registerObject(const char *uri, int major, int minor)
{
    // @uri Silk.HTML
    qmlRegisterType<Text>(uri, major, minor, "Text");
}

void QmlHandler::Private::load(const QUrl &url, QHttpRequest *request, QHttpReply *reply, const QString &message)
{
    QQmlComponent *component = new QQmlComponent(&engine, url, reply);
    connect(component, SIGNAL(destroyed(QObject *)), this, SLOT(componentDestroyed(QObject *)), Qt::QueuedConnection);
    exec(component, request, reply, message);
}

void QmlHandler::Private::exec(QQmlComponent *component, QHttpRequest *request, QHttpReply *reply, const QString &message)
{
    switch (component->status()) {
    case QQmlComponent::Null:
        // TODO: any check?
        break;
    case QQmlComponent::Error:
        qDebug() << component->errorString();
        emit q->error(500, request, reply, component->errorString());
        break;
    case QQmlComponent::Loading:
        component2request.insert(component, request);
        component2reply.insert(component, reply);
        component2message.insert(component, message);
        connect(component, SIGNAL(statusChanged(QQmlComponent::Status)), this, SLOT(statusChanged()), Qt::UniqueConnection);
        break;
    case QQmlComponent::Ready: {
        HttpObject *http = new HttpObject(this);
        http->remoteAddress(request->remoteAddress());
        http->method(QString::fromLatin1(request->method()));
        QUrl url(request->url());
        QString query(url.query());
        url.setQuery(QString());
        http->scheme(url.scheme());
        http->host(url.host());
        http->port(url.port());
        http->path(url.path());
        http->query(query);
        http->data(QString(request->readAll()));
        QList<HttpFileData *> files;
        foreach (QHttpFileData *file, request->files()) {
            files.append(new HttpFileData(file, http));
        }
        http->setFiles(files);

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

        QQmlContext *context = new QQmlContext(&engine, this);
        context->setContextProperty(QLatin1String("http"), http);

        QObject *o = component->create(context);

        SilkAbstractHttpObject *object = qobject_cast<SilkAbstractHttpObject*>(o);
        if (!object) {
            emit q->error(403, request, reply, request->url().toString());
            return;
        }
        object->setParent(http);

        if (object->property("contentType").isValid()) {
            component2object.insert(component, object);
            object2request.insert(object, request);
            object2reply.insert(object, reply);
            object2http.insert(object, http);
            object2context.insert(object, context);
            QCoreApplication::processEvents();
            if (!http->loading()) {
                close(object);
            } else {
                connect(http, SIGNAL(loadingChanged(bool)), this, SLOT(loadingChanged(bool)));
            }
        } else {
            q->emit error(403, request, reply, request->url().toString());
        }


        break; }
    }
}

void QmlHandler::Private::close(SilkAbstractHttpObject *object)
{
    if (object2request.contains(object) && object2reply.contains(object) && object2context.contains(object) && object2http.contains(object)) {
        QHttpRequest *request = object2request.take(object);
        QHttpReply *reply = object2reply.take(object);
        QQmlContext *context = object2context.take(object);
        HttpObject *http = object2http.take(object);

        QByteArray out = object->out();


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

        if (object->property("contentType").isValid()) {
            reply->setRawHeader("Content-Type", object->property("contentType").toByteArray());
        }

        QVariant prolog = object->property("prolog");
        if (prolog.isValid())
            reply->write(prolog.toByteArray());

//        qDebug() << out;
        if (request->method() == "GET" || request->method() == "POST") {
            reply->write(out);
        }
        reply->close();
        context->deleteLater();
        http->deleteLater();
    }
}

void QmlHandler::Private::loadingChanged(bool loading)
{
    if (!loading) {
        HttpObject *http = qobject_cast<HttpObject *>(sender());
        close(qobject_cast<SilkAbstractHttpObject *>(http->children().first()));
    }
}

void QmlHandler::Private::statusChanged()
{
    QQmlComponent *component = qobject_cast<QQmlComponent *>(sender());
    if (component2request.contains(component) && component2reply.contains(component)) {
        exec(component, component2request.take(component), component2reply.take(component), component2message.take(component));
    } else if (component2socket.contains(component)) {
        exec(component, component2socket.take(component), component2message.take(component));
    }
}

void QmlHandler::Private::componentDestroyed(QObject *object)
{
    static bool cache = SilkConfig::value("cache.qml").toBool();

    if (component2request.contains(object)) {
        component2request.take(object);
    }
    if (component2reply.contains(object)) {
        component2reply.take(object);
    }
    if (component2socket.contains(object)) {
        component2socket.take(object);
    }
    if (component2message.contains(object)) {
        component2message.take(object);
    }
    if (component2object.contains(object)) {
        QObject *o = component2object.take(object);
        object2request.take(o);
        object2reply.take(o);
        if (object2context.contains(o)) {
            object2context.take(o)->deleteLater();
        }
        if (object2http.contains(o)) {
            object2http.take(o)->deleteLater();
        }
    }
    if (!cache)
        clearQmlCache();
}

void QmlHandler::Private::clearQmlCache()
{
    engine.trimComponentCache();
}

void QmlHandler::Private::load(const QUrl &url, QWebSocket *socket, const QString &message)
{
    QQmlComponent *component = new QQmlComponent(&engine, url, socket);
    connect(component, SIGNAL(destroyed(QObject *)), this, SLOT(componentDestroyed(QObject *)), Qt::QueuedConnection);
    exec(component, socket, message);
}

void QmlHandler::Private::exec(QQmlComponent *component, QWebSocket *socket, const QString &message)
{
    static bool cache = SilkConfig::value("cache.qml").toBool();
    switch (component->status()) {
    case QQmlComponent::Null:
        // TODO: any check?
        break;
    case QQmlComponent::Error:
        qDebug() << component->errorString();
        emit q->error(500, socket, component->errorString());
        break;
    case QQmlComponent::Loading:
        component2socket.insert(component, socket);
        component2message.insert(component, message);
        connect(component, SIGNAL(statusChanged(QQmlComponent::Status)), this, SLOT(statusChanged()), Qt::UniqueConnection);
        break;
    case QQmlComponent::Ready: {
        WebSocketObject *object = qobject_cast<WebSocketObject*>(component->create());
        if (!object) {
            emit q->error(403, socket, socket->url().toString());
            return;
        }
        if (!cache)
            connect(object, SIGNAL(destroyed()), this, SLOT(clearQmlCache()), Qt::QueuedConnection);

        object->setWebSocket(socket);
        QCoreApplication::processEvents();
        object->remoteAddress(socket->remoteAddress());
        QUrl url(socket->url());
        QString query(url.query());
        url.setQuery(QString());
        object->scheme(url.scheme());
        object->host(url.host());
        object->port(url.port());
        object->path(url.path());
        object->query(query);

        QVariantMap requestHeader;
        foreach (const QByteArray &key, socket->rawHeaderList()) {
            requestHeader.insert(QString(key), QString(socket->rawHeader(key)));
        }
        object->requestHeader(requestHeader);

        QVariantMap cookies;
        foreach (const QNetworkCookie &cookie, socket->cookies()) {
            QVariantMap c;
            c.insert(QLatin1String("value"), QString::fromUtf8(cookie.value()));
            c.insert(QLatin1String("expires"), cookie.expirationDate());
            c.insert(QLatin1String("domain"), cookie.domain());
            c.insert(QLatin1String("path"), cookie.path());
            c.insert(QLatin1String("secure"), cookie.isSecure());
            c.insert(QLatin1String("session"), cookie.isSessionCookie());
            cookies.insert(QString::fromUtf8(cookie.name()), c);
        }
        object->requestCookies(cookies);

        if (!message.isEmpty()) object->message(message);
        QCoreApplication::processEvents();
        QMetaObject::invokeMethod(object, "ready");

        component2object.insert(component, object);
        break; }
    }
}

QmlHandler::QmlHandler(QObject *parent)
    : SilkAbstractMimeHandler(parent)
    , d(new Private(this))
{
}

bool QmlHandler::load(const QUrl &url, QHttpRequest *request, QHttpReply *reply, const QString &message)
{
    QFileInfo fileInfo;
    if (url.scheme() == "qrc") {
        fileInfo = QFileInfo(url.toString().mid(3));
    } else {
        fileInfo = QFileInfo(url.toLocalFile());
    }

    if (fileInfo.fileName().at(0).isUpper()) {
        return false;
    } else {
        if (!fileInfo.isReadable()){
            emit error(403, request, reply, request->url().toString());
        } else {
            d->load(url, request, reply, message);
        }
    }
    return true;
}

bool QmlHandler::load(const QUrl &url, QWebSocket *socket, const QString &message)
{
    QFileInfo fileInfo;
    if (url.scheme() == "qrc") {
        fileInfo = QFileInfo(url.toString().mid(3));
    } else {
        fileInfo = QFileInfo(url.toLocalFile());
    }

    if (fileInfo.fileName().at(0).isUpper()) {
        return false;
    } else {
        if (!fileInfo.isReadable()){
            emit error(403, socket, socket->url().toString());
        } else {
            d->load(url, socket, message);
        }
    }
    return true;
}

#include "qmlhandler.moc"

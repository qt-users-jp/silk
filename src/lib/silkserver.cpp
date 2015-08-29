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

#include "silkserver.h"
#include "silkconfig.h"
#include "silkserver_logging.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QMimeDatabase>
#include <QtCore/QPluginLoader>
#include <QtCore/QRegularExpression>
#include <QtCore/QUrl>

#include <qhttprequest.h>
#include <qhttpreply.h>
#include <qwebsocket.h>

#include <silkmimehandlerinterface.h>
#include <silkabstractmimehandler.h>
#include <silkprotocolhandlerinterface.h>
#include <silkabstractprotocolhandler.h>

typedef QPair<QRegularExpression, QString> RewriteRule;

class SilkServer::Private : public QObject
{
    Q_OBJECT
public:
    Private(SilkServer *parent);

private slots:
    void incomingConnection(QHttpRequest *request, QHttpReply *reply);
    void incomingConnection(QWebSocket *socket);

private:
    QString documentRootForRequest(const QUrl &url) const;
    void load(const QFileInfo &fileInfo, QHttpRequest *request, QHttpReply *reply, const QString &message = QString());
    void loadFile(const QFileInfo &fileInfo, QHttpRequest *request, QHttpReply *reply);
    void loadUrl(const QUrl &url, QHttpRequest *request, QHttpReply *reply, const QString &message = QString());
    void load(const QFileInfo &fileInfo, QWebSocket *socket, const QString &message = QString());
    void loadUrl(const QUrl &url, QWebSocket *socket, const QString &message = QString());

private slots:
    void error(int statusCode, QHttpRequest *request, QHttpReply *reply, const QString &message = QString());
    void error(int statusCode, QWebSocket *socket, const QString &message = QString());

private:
    SilkServer *q;
    QMimeDatabase mimeDatabase;
    QMap<QString, SilkAbstractMimeHandler*> mimeHandlers;
    QMap<QString, SilkAbstractProtocolHandler*> protocolHandlers;
    QList<RewriteRule> rewriteRules;
public:
    QMap<QString, QString> documentRoots;
};

SilkServer::Private::Private(SilkServer *parent)
    : QObject(parent)
    , q(parent)
{
    QDir appDir = QCoreApplication::applicationDirPath();
    QDir rootDir = appDir;
    QString appPath(SILK_APP_PATH);
    // up to system root path
    for (int i = 0; i < appPath.count(QLatin1Char('/')) + 1; i++) {
        rootDir.cdUp();
    }
#ifdef QT_STATIC
    {
        foreach (QObject *object, QPluginLoader::staticInstances()) {
            {
                SilkMimeHandlerInterface *plugin = qobject_cast<SilkMimeHandlerInterface *>(object);
                if (plugin) {
                    SilkAbstractMimeHandler *handler = plugin->handler(this);
                    connect(handler, SIGNAL(error(int,QHttpRequest*,QHttpReply*,QString)), this, SLOT(error(int,QHttpRequest*,QHttpReply*,QString)));
                    connect(handler, SIGNAL(error(int,QWebSocket*,QString)), this, SLOT(error(int,QWebSocket*,QString)));
                    foreach (const QString &key, plugin->keys()) {
                        mimeHandlers.insert(key, handler);
                    }
                }
            }
            {
                SilkProtocolHandlerInterface *plugin = qobject_cast<SilkProtocolHandlerInterface *>(object);
                if (plugin) {
                    SilkAbstractProtocolHandler *handler = plugin->handler(this);
                    foreach (const QString &key, plugin->keys()) {
                        protocolHandlers.insert(key, handler);
                    }
                }
            }
        }
    }
#else // QT_STATIC
    {
        QDir pluginsDir = rootDir;
        pluginsDir.cd(SILK_PLUGIN_PATH);
        pluginsDir.cd("mimehandler");
        foreach (const QString &lib, pluginsDir.entryList(QDir::Files)) {
            QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(lib));
            if (pluginLoader.load()) {
                QObject *object = pluginLoader.instance();
                if (object) {
                    SilkMimeHandlerInterface *plugin = qobject_cast<SilkMimeHandlerInterface *>(object);
                    if (plugin) {
                        SilkAbstractMimeHandler *handler = plugin->handler(this);
                        connect(handler, SIGNAL(error(int,QHttpRequest*,QHttpReply*,QString)), this, SLOT(error(int,QHttpRequest*,QHttpReply*,QString)));
                        connect(handler, SIGNAL(error(int,QWebSocket*,QString)), this, SLOT(error(int,QWebSocket*,QString)));
                        foreach (const QString &key, plugin->keys()) {
                            mimeHandlers.insert(key, handler);
                        }
                    } else {
                        qWarning() << object;
                    }
                } else {
                    qWarning() << Q_FUNC_INFO << __LINE__;
                }
            } else {
                qWarning() << pluginLoader.errorString() << pluginsDir.absoluteFilePath(lib);
            }
        }
        pluginsDir.cd("..");

        pluginsDir.cd("protocolhandler");
        foreach (const QString &lib, pluginsDir.entryList(QDir::Files)) {
            QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(lib));
            if (pluginLoader.load()) {
                QObject *object = pluginLoader.instance();
                if (object) {
                    SilkProtocolHandlerInterface *plugin = qobject_cast<SilkProtocolHandlerInterface *>(object);
                    if (plugin) {
                        SilkAbstractProtocolHandler *handler = plugin->handler(this);
                        foreach (const QString &key, plugin->keys()) {
                            protocolHandlers.insert(key, handler);
                        }
                    } else {
                        qWarning() << object;
                    }
                } else {
                    qWarning() << Q_FUNC_INFO << __LINE__;
                }
            } else {
                qWarning() << pluginLoader.errorString() << pluginsDir.absoluteFilePath(lib);
            }
        }
    }
#endif // QT_STATIC

    connect(q, SIGNAL(incomingConnection(QHttpRequest *, QHttpReply *)), this, SLOT(incomingConnection(QHttpRequest *, QHttpReply *)));
    connect(q, SIGNAL(incomingConnection(QWebSocket *)), this, SLOT(incomingConnection(QWebSocket *)));

    QString listenAddress = SilkConfig::value("listen.address").toString();
    QHostAddress address;
    if (listenAddress == QStringLiteral("*")) {
        address = QHostAddress::Any;
    } else if (listenAddress == QStringLiteral("localhost")) {
        address = QHostAddress::LocalHost;
    } else if (!address.setAddress(listenAddress)) {
        qWarning() << "The address" << listenAddress << "is not available.";
        QMetaObject::invokeMethod(QCoreApplication::instance(), "quit", Qt::QueuedConnection);
        return;
    }

    int port = SilkConfig::value("listen.port").toInt();

    QVariantMap roots = SilkConfig::value("contents").toMap();
    foreach (const QString &key, roots.keys()) {
        QString value = roots.value(key).toString();
        if (value.contains(":/")) {
            documentRoots.insert(key, value);
        } else {
            QFileInfo fileInfo(value);
            if (fileInfo.isRelative()) {
                if (SilkConfig::file().startsWith(QStringLiteral(":/"))) {
                    documentRoots.insert(key, rootDir.absoluteFilePath(value));
                } else {
                    QDir dir(SilkConfig::file());
                    dir.cdUp();
                    documentRoots.insert(key, dir.absoluteFilePath(value));
                }
            } else {
                documentRoots.insert(key, value);
            }
        }
    }
    foreach (const QVariant &val, SilkConfig::value("rewrite").toList()) {
        QVariantMap map = val.toMap();
        foreach (const QString &key, map.keys()) {
            rewriteRules.append(RewriteRule(QRegularExpression(key), map.value(key).toString()));
        }
    }



    if (q->listen(address, port)) {
        ssInfo() << "silk is running on" << port;
    } else {
        ssWarning() << q->errorString();
        QMetaObject::invokeMethod(QCoreApplication::instance(), "quit", Qt::QueuedConnection);
    }
}

QString SilkServer::Private::documentRootForRequest(const QUrl &url) const
{
    QString ret(":/contents");
    if (documentRoots.contains(url.host())) {
        ret = documentRoots.value(url.host());
    } else if (documentRoots.contains("*")) {
        ret = documentRoots.value("*");
    }
    return ret;
}

void SilkServer::Private::incomingConnection(QHttpRequest *request, QHttpReply *reply)
{
    ssRequest() << request;

    QVariant serverName = SilkConfig::value("server.name");
    if (serverName.isNull()) {
        reply->setRawHeader("Server", "Silk");
    } else {
        QString str = serverName.toString();
        if (!str.isEmpty()) {
            reply->setRawHeader("Server", str.toUtf8());
        }
    }
    QString str = request->url().toString();
    foreach (const RewriteRule &rule, rewriteRules) {
        QRegularExpressionMatch match = rule.first.match(str);
        if (match.hasMatch()) {
            QString url = rule.second;
            for (int i = 0; i <= match.lastCapturedIndex(); i++) {
                url = url.replace(QString("$%1").arg(i), match.captured(i));
            }
            request->setUrl(QUrl(url));
            break;
        }
    }

    QString documentRoot = documentRootForRequest(request->url());

    if (documentRoot.indexOf("://") > 0) {
        QUrl url(documentRoot);
        url.setPath(request->url().path());
        loadUrl(url, request, reply);
    } else {
        QString fileName(documentRoot + request->url().path());
        QFileInfo fileInfo(fileName);
        if (fileInfo.isDir()) {
            if (request->url().path().endsWith("/")) {
                fileName = fileName + QStringLiteral("/index.qml");
                fileInfo = QFileInfo(fileName);
            } else {
                QUrl url(request->url());
                url.setPath(url.path() + "/");
                error(301, request, reply, url.toString());
                return;
            }
        }

        if (fileInfo.exists()) {
            load(fileInfo, request, reply);
        } else {
            error(404, request, reply, request->url().toString());
            return;
        }
    }
    ssOk() << request << reply->size();
}

void SilkServer::Private::incomingConnection(QWebSocket *socket)
{
    ssRequest() << socket;

    QString str = socket->url().toString();
    foreach (const RewriteRule &rule, rewriteRules) {
        QRegularExpressionMatch match = rule.first.match(str);
        if (match.hasMatch()) {
            QString url = rule.second;
            for (int i = 0; i <= match.lastCapturedIndex(); i++) {
                url = url.replace(QString("$%1").arg(i), match.captured(i));
            }
            socket->setUrl(QUrl(url));
            break;
        }
    }

    QString documentRoot = documentRootForRequest(socket->url());

    if (documentRoot.indexOf("://") > 0) {
        QUrl url(documentRoot);
        url.setPath(socket->url().path());
        loadUrl(url, socket);
    } else {
        QString fileName(documentRoot + socket->url().path());
        QFileInfo fileInfo(fileName);
        if (fileInfo.isDir()) {
            if (socket->url().path().endsWith("/")) {
                fileName = fileName + QStringLiteral("/index.qml");
                fileInfo = QFileInfo(fileName);
            } else {
                QUrl url(socket->url());
                url.setPath(url.path() + "/");
                error(301, socket, url.toString());
                return;
            }
        }

        if (fileInfo.exists()) {
            load(fileInfo, socket);
        } else {
            error(404, socket, socket->url().toString());
            return;
        }
    }
    ssOk() << socket;
}

void SilkServer::Private::load(const QFileInfo &fileInfo, QHttpRequest *request, QHttpReply *reply, const QString &message)
{
    QStringList mimeTypeListNeedCharset;
    mimeTypeListNeedCharset << QStringLiteral("text/x-qml")
                            << QStringLiteral("text/css")
                            << QStringLiteral("text/html")
                            << QStringLiteral("text/plain")
                            << QStringLiteral("image/svg+xml")
                            << QStringLiteral("application/javascript")
                            << QStringLiteral("application/x-javascript")
                            << QStringLiteral("application/xml")
                            << QStringLiteral("application/atom")
                            << QStringLiteral("application/rss+xml")
                            << QStringLiteral("application/x-shockwave-flash");
    QMimeType mimeType = mimeDatabase.mimeTypeForFile(fileInfo.fileName(), QMimeDatabase::MatchExtension);
    QString mime = mimeType.name();
    QString contentType = mime;
    if (mimeTypeListNeedCharset.contains(mime)) {
        contentType.append(QStringLiteral("; charset=utf-8"));
    }
    reply->setStatus(200);
    reply->setRawHeader("Content-Type", contentType.toUtf8());
    if (!mimeHandlers.contains(mime)) {
        mime = mime.section(QLatin1Char('/'), 0, 0) + QStringLiteral("/*");
    }
    if (mimeHandlers.contains(mime)) {
        QUrl url;
        if (fileInfo.filePath().startsWith(":/")) {
            url = QUrl("qrc" + fileInfo.absoluteFilePath());
        } else {
            url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
        }
        bool ret = mimeHandlers[mime]->load(url, request, reply, message);
        if (!ret) {
            loadFile(fileInfo, request, reply);
        }
    } else {
        loadFile(fileInfo, request, reply);
    }
}

void SilkServer::Private::loadFile(const QFileInfo &fileInfo, QHttpRequest *request, QHttpReply *reply)
{
    if (fileInfo.fileName().startsWith(".")) {
        error(403, request, reply, request->url().toString());
    } else {
        // TODO cache
        QFile file(fileInfo.absoluteFilePath());
        if (file.open(QFile::ReadOnly)) {
            QDateTime lastModified = fileInfo.lastModified();
            if (lastModified.isNull()) {
                lastModified.setTime_t(0);
            }
            reply->setRawHeader("Last-Modified", lastModified.toUTC().toString("ddd, d, MMM yyyy hh:mm:ss UTC").toUtf8());
            while (!file.atEnd()) {
                reply->write(file.read(1024 * 1024));
            }
            file.close();
            reply->close();
        } else {
            error(403, request, reply, request->url().toString());
        }
    }
}

void SilkServer::Private::loadUrl(const QUrl &url, QHttpRequest *request, QHttpReply *reply, const QString &message)
{
    bool ret = false;
    if (protocolHandlers.contains(url.scheme())) {
        ret = protocolHandlers[url.scheme()]->load(url, request, reply, message);
    }
    if (!ret) {
        error(403, request, reply, request->url().toString());
    }
}

void SilkServer::Private::error(int statusCode, QHttpRequest *request, QHttpReply *reply, const QString &message)
{
    ssError() << request << statusCode << message;
    QString documentRoot = documentRootForRequest(request->url());
    if (QFile::exists(QString::fromUtf8("%1/errors/%2.qml").arg(documentRoot).arg(statusCode))) {
        load(QFileInfo(QString::fromUtf8("%1/errors/%2.qml").arg(documentRoot).arg(statusCode)), request, reply, message);
    } else {
        load(QFileInfo(QString::fromUtf8(":/errors/%2.qml").arg(statusCode)), request, reply, message);
    }
}

void SilkServer::Private::load(const QFileInfo &fileInfo, QWebSocket *socket, const QString &message)
{
    QMimeType mimeType = mimeDatabase.mimeTypeForFile(fileInfo.fileName(), QMimeDatabase::MatchExtension);
    QString mime = mimeType.name();
    if (mimeHandlers.contains(mime)) {
        QUrl url;
        if (fileInfo.filePath().startsWith(":/")) {
            url = QUrl("qrc" + fileInfo.absoluteFilePath());
        } else {
            url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
        }
        bool ret = mimeHandlers[mime]->load(url, socket, message);
        if (!ret) {
            error(401, socket, socket->url().toString());
        }
    } else {
        error(401, socket, socket->url().toString());
    }
}

void SilkServer::Private::loadUrl(const QUrl &url, QWebSocket *socket, const QString &message)
{
    bool ret = false;
    if (protocolHandlers.contains(url.scheme())) {
        ret = protocolHandlers[url.scheme()]->load(url, socket, message);
    }
    if (!ret) {
        error(403, socket, socket->url().toString());
    }
}

void SilkServer::Private::error(int statusCode, QWebSocket *socket, const QString &message)
{
    ssError() << socket << statusCode << message;

    // TODO: error handling
//    socket->close();
    socket->deleteLater();
}

SilkServer::SilkServer(QObject *parent)
    : QHttpServer(parent)
    , d(new Private(this))
{
}

const QMap<QString, QString> &SilkServer::documentRoots() const
{
    return d->documentRoots;
}

void SilkServer::setDocumentRoots(const QMap<QString, QString> &documentRoots)
{
    if (d->documentRoots == documentRoots) return;
    d->documentRoots = documentRoots;
    emit documentRootsChanged(documentRoots);
}

#include "silkserver.moc"

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

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QMimeDatabase>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQml/qqml.h>

#include "qhttprequest.h"
#include "qhttpreply.h"

#include "qtquickhttpobject.h"
#include "qtquickhttptextobject.h"
#include "qtquickhttprepeater.h"

#include "qtquickhttphtmltag.h"
#include "qtquickhttphtmlcomment.h"

#include "qtquickhttpcssrule.h"

#include "qtquickhttpjson.h"

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

private:
    QString documentRootForRequest(const QHttpRequest *request) const;
    void loadQml(const QFileInfo &fileInfo, QHttpRequest *request, QHttpReply *reply, const QString &message = QString());
    void execQml(QQmlComponent *component, QHttpRequest *request, QHttpReply *reply, const QString &message = QString());
    void loadFile(const QFileInfo &fileInfo, QHttpRequest *request, QHttpReply *reply);
    void error(int statusCode, QHttpRequest *request, QHttpReply *reply, const QString &message = QString());
    void close(QtQuickHttpObject *http);

private:
    Silk *q;
    QMimeDatabase mimeDatabase;
    QQmlEngine engine;
    QMap<QObject*, QString> component2root;
    QMap<QObject*, QHttpRequest*> component2request;
    QMap<QObject*, QHttpReply*> component2reply;
    QMap<QObject*, QString> component2message;
    QMap<QObject*, QtQuickHttpObject*> component2http;
    QMap<QObject*, QHttpReply*> http2reply;

public:
    QMap<QString, QString> documentRoots;
};

Silk::Private::Private(Silk *parent)
    : QObject(parent)
    , q(parent)
{
    documentRoots.insert("*", ":/contents");
    qmlRegisterType<QtQuickHttpAbstractObject>();
    qmlRegisterType<QtQuickHttpObject>("QtQuick.HTTP", 1, 1, "Http");
    qmlRegisterType<QtQuickHttpTextObject>("QtQuick.HTTP", 1, 1, "Text");
    qmlRegisterType<QtQuickHttpRepeater>("QtQuick.HTTP", 1, 1, "Repeater");

    qmlRegisterType<QtQuickHttpHtmlTag>("QtQuick.HTML", 4, 01, "Tag");
    qmlRegisterType<QtQuickHttpHtmlComment>("QtQuick.HTML", 4, 01, "Comment");

    qmlRegisterType<QtQuickHttpCssRule>("QtQuick.CSS", 2, 1, "CSSRule");

    qmlRegisterType<QtQuickHttpJson>("QtQuick.JSON", 1, 0, "Json");

    engine.addImportPath(":/imports");

    connect(q, SIGNAL(incomingConnection(QHttpRequest *, QHttpReply *)), this, SLOT(incomingConnection(QHttpRequest *, QHttpReply *)));
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
        QtQuickHttpObject *http = qobject_cast<QtQuickHttpObject *>(component->create());
        QUrl url(request->url());
        QString query(url.query());
        url.setQuery(QString());
        http->url(url);
        QVariantMap requestHeader;
        foreach (const QByteArray &key, request->rawHeaderList()) {
            requestHeader.insert(QString(key), QString(request->rawHeader(key)));
        }
        http->requestHeader(requestHeader);
        http->query(query);
        http->data(QString(request->readAll()));
        if (!message.isEmpty()) http->message(message);
        QMetaObject::invokeMethod(http, "ready");

        component2http.insert(component, http);
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
        QtQuickHttpObject *http = component2http.take(object);
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
        QtQuickHttpObject *http = qobject_cast<QtQuickHttpObject *>(sender());
        close(http);
    }
}

void Silk::Private::close(QtQuickHttpObject *http)
{
    if (http2reply.contains(http)) {
        QHttpReply *reply = http2reply.take(http);
        reply->setStatus(http->status());

        QVariantMap header = http->responseHeader();
        foreach (const QString &key, header.keys()) {
            QString value = header.value(key).toString();
            reply->setRawHeader(key.toUtf8(), value.toUtf8());
        }
        reply->write(http->out());
        reply->close();
    }
    http->deleteLater();
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

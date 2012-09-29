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

#include "silkapplication.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtQml/qqml.h>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlEngine>

#include <silkconfig.h>
#include <silkimportsinterface.h>

class SilkApplication::Private : public QObject
{
    Q_OBJECT
public:
    Private(SilkApplication *parent);

private:
    SilkApplication *q;
    QQmlEngine engine;
};

SilkApplication::Private::Private(SilkApplication *parent)
    : QObject(parent)
    , q(parent)
{
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
    engine.setOfflineStoragePath(appDir.absoluteFilePath(SilkConfig::value("storage.path").toString()));
    engine.addImportPath(":/imports");
    foreach (const QString &importPath, SilkConfig::value("import.path").toStringList()) {
        engine.addImportPath(appDir.absoluteFilePath(importPath));
    }

    QVariantList tasks = SilkConfig::value("silk.tasks").toList();

    foreach (const QVariant &task, tasks) {
        QQmlComponent *component = new QQmlComponent(&engine, QUrl::fromLocalFile(task.toString()), this);
        switch (component->status()) {
        case QQmlComponent::Null:
            break;
        case QQmlComponent::Error:
            qDebug() << component->errorString();
            QMetaObject::invokeMethod(q, "quit", Qt::QueuedConnection);
            break;
        case QQmlComponent::Loading:
            break;
        case QQmlComponent::Ready: {
            QObject *app = component->create();
            connect(app, SIGNAL(destroyed(QObject *)), q, SLOT(quit()), Qt::QueuedConnection);
            break; }
        }
    }
}

SilkApplication::SilkApplication(int &argc, char **argv)
    : QCoreApplication(argc, argv)
    , d(new Private(this))
{
}

#include "silkapplication.moc"

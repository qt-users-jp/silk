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

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QJsonDocument>
#include <QtCore/QStringList>

#include "silkservice.h"
#include "silk.h"

QVariantMap readConfigFile(const QString &fileName)
{
    QVariantMap ret;
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
        file.close();
        if (error.error == QJsonParseError::NoError) {
            ret = doc.toVariant().toMap();
        } else {
            qWarning() << error.offset << error.errorString();
        }
    } else {
        qWarning() << file.error() << file.errorString();
    }
    return ret;
}

QVariantMap readConfig(int argc, char **argv)
{
    QVariantMap ret;
    QString fileName = QDir::home().absoluteFilePath(".silkrc");
    for (int i = 1; i < argc; i++) {
        if (QString::fromUtf8(argv[i]) == QLatin1String("--config")) {
            if (argc - i > 1) {
                fileName = argv[++i];
                break;
            }
        }
    }

    if (QFile::exists(fileName)) {
        QVariantMap ret2 = readConfigFile(fileName);
        foreach (const QString &key, ret2.keys()) {
            ret.insert(key, ret2.value(key));
        }
    } else {
        qWarning() << fileName;
    }
    return ret;
}

int main(int argc, char *argv[])
{
    int ret = 0;
    QVariantMap defaultConfig = readConfigFile(":/silkrc.default");
    QVariantMap config = readConfig(argc, argv);
    QString serviceName("SilkServer");
    if (config.contains("name")) {
        serviceName.append("-").append(config.value("name").toString());
    }

    QString serviceFilePath;
    bool install = false;
    bool uninstall = false;
    bool daemon = false;
    bool kill = false;
    for (int i = 1; i < argc; i++) {
        QString opt(argv[i]);
        if (opt == QLatin1String("-d")) {
            daemon = true;
        } else if (opt == QLatin1String("-i")) {
            if (i < argc) {
                install = true;
                i++;
                serviceFilePath = QString(argv[i]);
            } else {
                // error
            }
        } else if (opt == QLatin1String("-u")) {
            uninstall = true;
        } else if (opt == QLatin1String("-k")) {
            kill = true;
        }
    }

    QtServiceController controller(serviceName);
    if (daemon) {
        if (!controller.isInstalled()) {
            if (install) {
                controller.install(serviceFilePath);
            }
        }

        SilkService service(argc, argv, serviceName);
        QMap<QString, QString> documentRoots;
        int port = defaultConfig.value("port").toInt();
        if (config.contains("port")) {
            bool ok;
            port = config.value("port").toInt(&ok);
            if (!ok) {
                port = defaultConfig.value("port").toInt();
            }
        }
        if (config.contains("roots")) {
            QVariant roots = config.value("roots");
            if (roots.type() == QVariant::Map) {
                QVariantMap rootsMap = config.value("roots").toMap();
                foreach (const QString &key, rootsMap.keys()) {
                    documentRoots.insert(key, rootsMap.value(key).toString());
                }
            }
        }
        if (documentRoots.isEmpty()) {
            QVariantMap rootsMap = defaultConfig.value("roots").toMap();
            foreach (const QString &key, rootsMap.keys()) {
                documentRoots.insert(key, rootsMap.value(key).toString());
            }
        }
        service.setServerInfo(documentRoots, QHostAddress::Any, port);
        ret = service.exec();
    } else if (install) {
        if (controller.isInstalled()) {
            if (controller.serviceFilePath() != serviceFilePath) {
                controller.uninstall();
            } else {
                return ret;
            }
        }
        controller.install(serviceFilePath);
    } else if (uninstall) {
        if (controller.isInstalled()) controller.uninstall();
    } else if (kill) {
        if (controller.isRunning()) controller.stop();
    } else {
        QCoreApplication app(argc, argv);

        Silk server;
        QMap<QString, QString> documentRoots;
        int port = 8080;
        if (config.contains("port")) {
            bool ok;
            port = config.value("port").toInt(&ok);
            if (!ok) {
                port = 8080;
            }
        }

        if (config.contains("roots")) {
            QVariantMap roots = config.value("roots").toMap();
            foreach (const QString &key, roots.keys()) {
                documentRoots.insert(key, roots.value(key).toString());
            }
        }
        if (documentRoots.isEmpty()) {
            documentRoots.insert("*", ":/contents");
        }
        server.setDocumentRoots(documentRoots);
        if (server.listen(QHostAddress::Any, port)) {
            ret = app.exec();
        } else {
            qWarning() << server.errorString();
            ret = -1;
        }
    }
    return ret;
}

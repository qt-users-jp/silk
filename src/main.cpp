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
#include <QtCore/QStringList>
#include <QtCore/QDebug>

#include "silkservice.h"
#include "silk.h"

int setupServerInfo(int argc, char **argv, QMap<QString, QString> *documentRoots) {
    int ret = 8080;

    for (int i = 0; i < argc; i++) {
        if (QString::fromUtf8(argv[i]) == QLatin1String("--root")) {
            if (argc - i > 2) {
                QString key = QString::fromUtf8(argv[++i]);
                QString value = QString::fromUtf8(argv[++i]);
                documentRoots->insert(key, value);
            }
        } else if (QString::fromUtf8(argv[i]) == QLatin1String("--port")) {
            if (argc - i > 1) {
                ret = QString::fromUtf8(argv[++i]).toInt();
            }
        }
    }

    if (documentRoots->isEmpty()) {
        documentRoots->insert("*", ":/contents");
    }

    return ret;
}

int main(int argc, char *argv[])
{
    int ret = 0;
    QString serviceName("SilkServer");

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
        int port = setupServerInfo(argc, argv, &documentRoots);
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

        QMap<QString, QString> documentRoots;
        int port = setupServerInfo(argc, argv, &documentRoots);
        Silk server;
        server.setDocumentRoots(documentRoots);
        server.listen(QHostAddress::Any, port);

        ret = app.exec();
    }
    return ret;
}

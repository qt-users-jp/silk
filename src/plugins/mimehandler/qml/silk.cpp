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

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QUuid>

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>

Silk::Silk(QObject *parent)
    : QObject(parent)
{
}

QString Silk::uuid()
{
    QString ret = QUuid::createUuid().toString().mid(1);
    ret.chop(1);
    return ret;
}

QString Silk::readFile(const QString &filePath) const
{
    QString ret;
    QFile file(filePath);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        ret = QString::fromUtf8(file.readAll());
        file.close();
    }
    return ret;
}

QVariantList Silk::readDir(const QString &path) const
{
    QVariantList ret;
    QDir dir(path);
    foreach (const QString &file, dir.entryList(QDir::Files)) {
        ret.append(file);
    }
    return ret;
}

bool Silk::isAvailable(const QString &uri, int major, int minor, const QString &element) const
{
    bool ret = false;
    QQmlEngine *engine = qobject_cast<QQmlEngine*>(parent());
    QQmlContext *context = new QQmlContext(engine->rootContext());
    QQmlComponent component(engine);
    QByteArray data = QString("import %1 %2.%3\n%4 {}").arg(uri).arg(major).arg(minor).arg(element).toUtf8();
    component.setData(data, QUrl());
    QObject *object = component.create(context);
    object->deleteLater();
    ret = !component.isError();

    return ret;
}

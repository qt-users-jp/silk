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

#include "httpobject.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTemporaryFile>
#include <qhttprequest.h>

HttpFileData::HttpFileData(QHttpFileData *data, QObject *parent)
    : QObject(parent)
{
    fileName(data->fileName());
    contentType(data->contentType());
    QTemporaryFile file;
    if (file.open()) {
        file.setAutoRemove(false);
        file.write(data->readAll());
        file.close();
        filePath(file.fileName());
    }
}

bool HttpFileData::save(const QString &as) const
{
    bool ret = false;
    QFile file(as);
    if (file.open(QFile::WriteOnly)) {
        file.write(m_data->readAll());
        file.close();
        ret = true;
    }
    return ret;
}

bool HttpFileData::remove()
{
    bool ret = QFile::remove(m_filePath);
    if (ret)
        filePath(QString());
    return ret;
}

HttpObject::HttpObject(QObject *parent)
    : SilkAbstractHttpObject(parent)
    , m_loading(false)
    , m_status(200)
    , m_escapeHTML(false)
{
}

QQmlListProperty<HttpFileData> HttpObject::files()
{
    return QQmlListProperty<HttpFileData>(this, m_files);
}

void HttpObject::setFiles(const QList<HttpFileData *> &files)
{
    m_files = files;
}

QByteArray HttpObject::out()
{
    QByteArray ret;
    foreach (QObject *child, contentsList()) {
        SilkAbstractHttpObject *object = qobject_cast<SilkAbstractHttpObject *>(child);
        if (object && object->enabled()) {
            if (!m_responseHeader.contains("Content-Type")) {
                QVariant contentType = object->property("contentType");
                if (!contentType.isNull()) {
                    m_responseHeader.insert(QLatin1String("Content-Type"), contentType);
                }
            }
            ret.append(object->out());
        }
    }
    return ret;
}

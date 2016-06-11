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
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtHttpServer/QHttpRequest>

HttpFileData::HttpFileData(QHttpFileData *data, QObject *parent)
    : QObject(parent)
{
    fileName(data->fileName());
    contentType(data->contentType());
    if (m_file.open()) {
        m_file.write(data->readAll());
        m_file.close();
        filePath(m_file.fileName());
    }
}

bool HttpFileData::save(const QString &fileName)
{
    bool ret = false;
    QFileInfo fi(fileName);
    if (QDir::root().mkpath(fi.dir().absolutePath()))
        ret = m_file.copy(fileName);
    return ret;
}

HttpObject::HttpObject(QObject *parent)
    : QObject(parent)
    , m_port(-1)
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

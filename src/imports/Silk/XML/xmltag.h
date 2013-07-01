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

#ifndef XMLTAG_H
#define XMLTAG_H

#include <silkabstracthttpobject.h>

class XmlTag : public SilkAbstractHttpObject
{
    Q_OBJECT

    Q_PROPERTY(QString contentType READ contentType WRITE contentType NOTIFY contentTypeChanged)
    Q_PROPERTY(QString prolog READ prolog WRITE prolog NOTIFY prologChanged)
    Q_PROPERTY(QString tagName READ tagName WRITE tagName NOTIFY tagNameChanged)
    Q_PROPERTY(QString text READ text WRITE text NOTIFY textChanged)
public:
    explicit XmlTag(QObject *parent = 0);
    
    virtual QByteArray out();

signals:
    void prologChanged(const QString &prolog);
    void contentTypeChanged(const QString &contentType);
    void tagNameChanged(const QString &tagName);
    void textChanged(const QString &text);

private:
    Q_DISABLE_COPY(XmlTag)
    SILK_ADD_PROPERTY(const QString &, contentType, QString)
    SILK_ADD_PROPERTY(const QString &, prolog, QString)
    SILK_ADD_PROPERTY(const QString &, tagName, QString)
//    SILK_ADD_PROPERTY(const QString &, text, QString)
public:
    const QString & text() const;
    void text(const QString & text);
private:
    QString m_text;
};

#endif // XMLTAG_H

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

#include "xmltag.h"

#include <QtCore/QDebug>
#include <QtCore/QMetaProperty>

XmlTag::XmlTag(QObject *parent)
    : SilkAbstractHttpObject(parent)
    , m_contentType(QStringLiteral("application/xml; charset=utf-8;"))
    , m_prolog(QStringLiteral("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"))
{
}

QString XmlTag::out()
{
    QString ret;

    bool tagNameIsEmpty = tagName().isEmpty();
    QString text;
    QString attributes;

    int count = metaObject()->propertyCount();
    for (int i = 0; i < count; i++) {
        const QMetaProperty &p = metaObject()->property(i);
        QString key(p.name());
        if (key == QStringLiteral("prolog")) continue;

        bool skip = false;
        for (int i = 0; i < key.length(); i++) {
            if (key.at(i).isUpper()) {
                skip = true;
                break;
            }
        }
        if (skip) continue;

        if (key.startsWith(QStringLiteral("__"))) continue;
        if (key.startsWith(QLatin1Char('_')))
            key = key.mid(1);
        key.replace(QStringLiteral("__"), QStringLiteral(":"));
        key.replace(QLatin1Char('_'), QLatin1Char('-'));

        switch (p.type()) {
        case QVariant::String: {
            QString value = p.read(this).toString();
            if (key == QStringLiteral("text")) {
                text = value;
            } else if (!value.isNull()){
                if (!value.isEmpty()) {
                    attributes.append(QString(" %1=\"%2\"").arg(key).arg(value));
                }
            }
            break; }
        case QVariant::Bool: {
//            bool value = p.read(this).toBool();
//            if (key == QStringLiteral("enabled")) {

//            } else {
//                // TODO support key="key" style
//            }
            break; }
        default:
            break;
        }
    }

    if (!tagNameIsEmpty) {
        ret.append(QString("<%1%2").arg(tagName()).arg(attributes));
    }

    bool hasChildObjects = false;

    if (!text.isNull()) {
        hasChildObjects = true;
        if (!tagNameIsEmpty)
            ret.append(QLatin1Char('>'));
        ret.append(text);
    }

    foreach (QObject *child, contentsList()) {
        SilkAbstractHttpObject *object = qobject_cast<SilkAbstractHttpObject *>(child);
        if (object && object->enabled()) {
            if (!hasChildObjects) {
                if (!tagNameIsEmpty)
                    ret.append(QLatin1Char('>'));
                hasChildObjects = true;
            }
            ret.append(object->out());
        }
    }

    if (!tagNameIsEmpty) {
        if (hasChildObjects) {
            ret.append(QString("</%1>").arg(tagName()));
        } else if (!text.isNull()){
            ret.append(QString("></%1>").arg(tagName()));
        } else {
            ret.append(QStringLiteral(" />"));
        }
    }

    QVariant escapeHTML = property("escapeHTML");
    if (escapeHTML.isValid() && escapeHTML.toBool()) {
        ret.replace(QStringLiteral("&"), QStringLiteral("&amp;"));
        ret.replace(QStringLiteral("<"), QStringLiteral("&lt;"));
        ret.replace(QStringLiteral(">"), QStringLiteral("&gt;"));
    }
    return ret;
}

const QString & XmlTag::text() const
{
    return m_text;
}

void XmlTag::text(const QString & text) {
    if (m_text == text && m_text.isEmpty() == text.isEmpty() && m_text.isNull() == text.isNull()) return;
    m_text = text;
    emit textChanged(text);
}

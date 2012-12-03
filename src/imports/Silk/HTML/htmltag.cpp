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

#include "htmltag.h"

#include <QtCore/QDebug>
#include <QtCore/QMetaProperty>
#include <QtCore/QStringList>

HtmlTag::HtmlTag(QObject *parent)
    : SilkAbstractHttpObject(parent)
{
}

QByteArray HtmlTag::out()
{
    QByteArray ret;

    QString text;
    QStringList attributes;

    int count = metaObject()->propertyCount();
    for (int i = 0; i < count; i++) {
        QMetaProperty p = metaObject()->property(i);
        QString key(p.name());
        if (key != key.toLower()) continue;
        if (key.startsWith("__")) continue;
        if (key.startsWith("_"))
            key = key.mid(1);
        key.replace("__", ":");
        key.replace('_', "-");

        switch (p.type()) {
        case QVariant::String: {
            QString value = p.read(this).toString();

            if (key == QLatin1String("text")) {
                text = value;
            } else if (!value.isNull()){
                if (value.isEmpty()) {
//                    attributes.append(QString(" %1").arg(key));
                } else {
                    attributes.append(QString(" %1=\"%2\"").arg(key).arg(value));
                }
            }
            break; }
        case QVariant::Bool: {
//            bool value = p.read(this).toBool();
            if (key == QLatin1String("enabled")) {

            } else {
                // TODO support key="key" style
            }
            break; }
        default:
            break;
        }
    }

    if (!tagName().isEmpty())
        ret.append(QString("<%1%2").arg(tagName()).arg(attributes.join("")));

    bool hasChildObjects = false;

    if (text.isEmpty()) {
        foreach (QObject *child, contentsList()) {
            SilkAbstractHttpObject *object = qobject_cast<SilkAbstractHttpObject *>(child);
            if (object && object->enabled()) {
                if (!hasChildObjects) {
                    if (!tagName().isEmpty())
                        ret.append(">");
                    hasChildObjects = true;
                }
                ret.append(object->out());
            }
        }
    } else {
        hasChildObjects = true;
        if (!tagName().isEmpty())
            ret.append(">");
        ret.append(text);
    }

    if (!tagName().isEmpty()) {
        if (hasChildObjects) {
            ret.append(QString("</%1>").arg(tagName()));
        } else if (!text.isNull()){
            ret.append(QString("></%1>").arg(tagName()));
        } else {
            ret.append(QLatin1String(" />"));
        }
    }

    QVariant escapeHTML = property("escapeHTML");
    if (escapeHTML.isValid() && escapeHTML.toBool()) {
        QString str = QString::fromUtf8(ret);
        str.replace("&", "&amp;");
        str.replace("<", "&lt;");
        str.replace(">", "&gt;");
        ret = str.toUtf8();
    }
    return ret;
}

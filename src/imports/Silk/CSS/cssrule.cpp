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

#include "cssrule.h"

#include <QtCore/QDebug>
#include <QtCore/QMetaProperty>
#include <QtCore/QStringList>

CssRule::CssRule(QObject *parent)
    : SilkAbstractHttpObject(parent)
    , m_selector()
{
}

QByteArray CssRule::out()
{
    // TODO: support A { ...; Rule {... } }
    QByteArray ret;
    QList<QByteArray> list;
    generate(list);
    foreach (const QByteArray &l, list) {
        ret.append(l);
    }
    return ret;
}

void CssRule::generate(QList<QByteArray>& list, const QStringList &selectors) const
{
    QStringList attributes = parseAttributes(this);

    foreach (const QObject *child, contentsList()) {
        const CssRule *css = qobject_cast<const CssRule *>(child);
        if (css && css->enabled() && css->selector().isEmpty()) {
            attributes.append(parseAttributes(css));
        }
    }

    QStringList newSelectors;
    if (selectors.isEmpty()) {
        if (m_selector.startsWith("@media ")) {
            QList<QByteArray> list2;
            foreach (const QObject *child, contentsList()) {
                const CssRule *css = qobject_cast<const CssRule *>(child);
                if (css && css->enabled() && !css->selector().isEmpty()) {
                    css->generate(list2);
                }
            }
            QString ret;
            foreach (const QByteArray &l, list2) {
                ret.append(QString::fromUtf8(l));
            }
            ret.replace("\r\n", "\r\n    ");
            list.append(QString("%1 {\r\n    %2\r\n}\r\n").arg(m_selector).arg(ret).toUtf8());
        } else {
            if (!attributes.isEmpty()) {
                list.append(QString("%1 {\r\n    %2\r\n}\r\n").arg(m_selector).arg(attributes.join("\r\n    ")).toUtf8());
            }
            newSelectors.append(m_selector);
            foreach (const QObject *child, contentsList()) {
                const CssRule *css = qobject_cast<const CssRule *>(child);
                if (css && css->enabled() && !css->selector().isEmpty()) {
                    css->generate(list, newSelectors);
                }
            }
        }
    } else {
        foreach (const QString &s, selectors) {
            QString newSelector;
            foreach (const QString &s2, m_selector.split(',')) {
                if (s2.startsWith(".") || s2.startsWith("#") || s2.startsWith(":") || s2.startsWith("[")) {
                    newSelector = QString("%1%2").arg(s).arg(s2);
                } else if (s2.startsWith(">") || s2.startsWith("+")) {
                        newSelector = QString("%1 %2").arg(s).arg(s2);
                } else if (!s.isEmpty()){
                    newSelector = QString("%1 %2").arg(s).arg(s2);
                } else {
                    newSelector = s2;
                }
                if (!attributes.isEmpty()) {
                    list.append(QString("%1 {\r\n    %2\r\n}\r\n").arg(newSelector).arg(attributes.join("\r\n    ")).toUtf8());
                }
            }
            newSelectors.append(newSelector);
        }

        foreach (const QObject *child, contentsList()) {
            const CssRule *css = qobject_cast<const CssRule *>(child);
            if (css && css->enabled() && !css->selector().isEmpty()) {
                css->generate(list, newSelectors);
            }
        }
    }
}

QStringList CssRule::parseAttributes(const CssRule *css) const
{
    QStringList ret;

    int count = css->metaObject()->propertyCount();
    for (int i = 0; i < count; i++) {
        QMetaProperty p = css->metaObject()->property(i);
        QString key(p.name());
        if (key != key.toLower()) continue;
        if (key.startsWith("__")) continue;
        if (key.startsWith("_float")) key = key.mid(1);
        key.replace('_', "-");

        switch (p.type()) {
        case QVariant::String: {
            QString value = p.read(css).toString();

            if (key == QLatin1String("selector")) {
            } else if (key == QLatin1String("objectName")) {
            } else if (!value.isEmpty()){
                ret.append(QString("%1: %2;").arg(key).arg(value));
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

    return ret;
}


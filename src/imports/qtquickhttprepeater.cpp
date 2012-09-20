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

#include "qtquickhttprepeater.h"

#include <QtCore/QDebug>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>

QtQuickHttpRepeater::QtQuickHttpRepeater(QObject *parent)
    : QtQuickHttpAbstractObject(parent)
{
}

QByteArray QtQuickHttpRepeater::out() const
{
    QByteArray ret;
    QVariantList list;

    switch (m_model.type()) {
    case QVariant::Int: {
        int count = m_model.toInt();
        for (int i = 0; i < count; i++) {
            QVariantMap model;
            model.insert("modelData", i);
            list.append(model);
        }
        break; }
    case QVariant::List: {
        QVariantList l = m_model.toList();
        foreach (const QVariant &v, l) {
            QVariantMap model;
            switch (v.type()) {
            case QVariant::Map:
                list.append(v);
                break;
            case QVariant::String:
            case QVariant::Int:
                model.insert("modelData", v);
                break;
            default:
                qDebug() << v.type() << v;
                break;
            }
            if (!model.isEmpty())
                list.append(model);
        }
        break; }
    case QVariant::Map: {
        QVariantMap m = m_model.toMap();
        foreach (const QString &key, m.keys()) {
            QVariantMap model;
            model.insert("key", key);
            model.insert("value", m.value(key));
            list.append(model);
        }
        break; }
    default: {
        qDebug() << Q_FUNC_INFO << __LINE__ << m_model.type() << m_model;
//        QObject *object = qvariant_cast<QObject*>(model);
//        if (object) {
//            qDebug() << object;
//        }
        break; }
    }


    QQmlContext *context = qmlEngine(this)->rootContext();
    QVariant model = context->contextProperty("model");
    foreach (const QVariant &model, list) {
        context->setContextProperty("model", model);

        // TODO: workaround for duplication of Component
        QList<QObject *> done;

        foreach (QObject *object, contentsList()) {
            if (done.contains(object)) continue;
            done.append(object);

            QtQuickHttpAbstractObject *http = qobject_cast<QtQuickHttpAbstractObject *>(object);
            if (!http) {
                QQmlComponent *component = qobject_cast<QQmlComponent *>(object);
                QObject *obj = component->create(context);
                http = qobject_cast<QtQuickHttpAbstractObject *>(obj);
                obj->deleteLater();
            }
            if (http) {
                ret.append(http->out());
            }
        }
    }
    context->setContextProperty("model", model);
    return ret;
}

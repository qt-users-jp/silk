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

#include "repeater.h"

#include <QtCore/QDebug>
#include <QtCore/QAbstractListModel>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>

Repeater::Repeater(QObject *parent)
    : SilkAbstractHttpObject(parent)
{
}

QByteArray Repeater::out()
{
    QByteArray ret;
    QVariantList list;

    switch (static_cast<int>(m_model.type())) {
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
    case QMetaType::QObjectStar: {
        QAbstractListModel *m = qobject_cast<QAbstractListModel*>(qvariant_cast<QObject*>(m_model));
        if (m) {
            QHash<int, QByteArray> roleNames = m->roleNames();
            for (int i = 0; i < m->rowCount(); i++) {
                QVariantMap model;
                foreach (int role, roleNames.keys()) {
                    QVariant data = m->data(m->index(i), role);
                    model.insert(roleNames.value(role), data);
                }
                list.append(model);
            }
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
    foreach (const QVariant &m, list) {
        context->setContextProperty("model", m);

        // TODO: workaround for duplication of Component
        QList<QObject *> done;

        foreach (QObject *object, contentsList()) {
            if (done.contains(object)) continue;
            done.append(object);
            QQmlComponent *component = qobject_cast<QQmlComponent *>(object);
            if (component) {
                QObject *obj = component->create(context);
                SilkAbstractHttpObject *http = qobject_cast<SilkAbstractHttpObject *>(obj);
                if (http) {
                    ret.append(http->out());
                }
                delete obj;
            }
        }
    }
    context->setContextProperty("model", model);
    return ret;
}

QQmlListProperty<QQmlComponent> Repeater::contents()
{
    return QQmlListProperty<QQmlComponent>(this, m_contents);
}

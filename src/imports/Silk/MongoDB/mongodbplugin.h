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

#ifndef MONGODBPLUGIN_H
#define MONGODBPLUGIN_H

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <silkimportsinterface.h>

#include "mongodatabase.h"
#include "mongocollection.h"
#include "mongoquery.h"

class MongoDBPlugin : public QObject, SilkImportsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "me.qtquick.silk.imports")
    Q_INTERFACES(SilkImportsInterface)
public:
    virtual QString name() const { return QLatin1String("mongodb"); }

    virtual void silkRegisterObject() {
        silkRegisterObject("Silk.MongoDB", 0, 1);
    }

public slots:
    virtual void silkRegisterObject(const char *uri, int major, int minor) {
        // @uri Silk.MongoDB
        qmlRegisterType<MongoDatabase>(uri, major, minor, "Database");
        qmlRegisterType<MongoCollection>(uri, major, minor, "Collection");
        qmlRegisterUncreatableType<MongoQuery>(uri, major, minor, "Query", "use .find()");
    }

signals:
    void registerObject(const char *uri, int major, int minor);
};

#endif // MONGODBPLUGIN_H
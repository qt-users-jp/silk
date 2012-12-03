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

#include "carpetabstractmodel.h"

#include <QtCore/QDebug>

class CarpetAbstractModel::Private
{
public:
    QList<CarpetFileInfo> data;
};

CarpetAbstractModel::CarpetAbstractModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new Private)
{
}

CarpetAbstractModel::~CarpetAbstractModel()
{
    delete d;
}

QHash<int, QByteArray> CarpetAbstractModel::roleNames() const
{
    static QHash<int, QByteArray> ret;
    if (ret.isEmpty()) {
        ret.insert(IdRole, "id");
        ret.insert(NameRole, "name");
        ret.insert(DirectoryRole, "dir");
        ret.insert(PathRole, "path");
        ret.insert(MimeTypeRole, "mime");
        ret.insert(SizeRole, "size");
        ret.insert(HashRole, "hash");
        ret.insert(PermissionsRole, "permissions");
        ret.insert(OwnerIdRole, "ownerId");
        ret.insert(GroupIdRole, "groupId");
        ret.insert(CTimeRole, "ctime");
        ret.insert(MTimeRole, "mtime");
        ret.insert(ATimeRole, "atime");
    }
    return ret;
}

int CarpetAbstractModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return d->data.count();
}

QVariant CarpetAbstractModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= d->data.count()) return QVariant();
    CarpetFileInfo fileInfo = d->data.at(index.row());
    switch (role) {
    case IdRole:
        return fileInfo.id;
    case NameRole:
        return fileInfo.name;
    case DirectoryRole:
        return fileInfo.dir;
    case PathRole:
        return fileInfo.path;
    case MimeTypeRole:
        return fileInfo.mimeType;
    case SizeRole:
        return fileInfo.size;
    case HashRole:
        return fileInfo.hash;
    case PermissionsRole:
        return fileInfo.permissions;
    case OwnerIdRole:
        return fileInfo.ownerId;
    case GroupIdRole:
        return fileInfo.groupId;
    case CTimeRole:
        return fileInfo.cTime;
    case MTimeRole:
        return fileInfo.mTime;
    case ATimeRole:
        return fileInfo.aTime;
    default:
        break;
    }
    return QVariant();
}

void CarpetAbstractModel::append(const CarpetFileInfo &fileInfo)
{
    int size = d->data.size();
    beginInsertRows(QModelIndex(), size, size);
    d->data.append(fileInfo);
    endInsertRows();
}

QVariantMap CarpetAbstractModel::get(int i) const
{
    QVariantMap ret;
    if (i >= d->data.count()) return ret;
    CarpetFileInfo fileInfo = d->data.at(i);
    ret.insert("id", fileInfo.id);
    ret.insert("name", fileInfo.name);
    ret.insert("dir", fileInfo.dir);
    ret.insert("path", fileInfo.path);
    ret.insert("mime", fileInfo.mimeType);
    ret.insert("size", fileInfo.size);
    ret.insert("hash", fileInfo.hash);
    ret.insert("permissions", fileInfo.permissions);
    ret.insert("ownerId", fileInfo.ownerId);
    ret.insert("groupId", fileInfo.groupId);
    ret.insert("ctime", fileInfo.cTime);
    ret.insert("mtime", fileInfo.mTime);
    ret.insert("atime", fileInfo.aTime);
    return ret;
}

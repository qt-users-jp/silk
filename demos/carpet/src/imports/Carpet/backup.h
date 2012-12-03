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

#ifndef BACKUP_H
#define BACKUP_H

#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QVariantList>

class QFileInfo;

#define ADD_PROPERTY(type, name, type2) \
public: \
    type name() const { return m_##name; } \
    void name(type name) { \
        if (m_##name == name) return; \
        m_##name = name; \
        emit name##Changed(name); \
    } \
private: \
    type2 m_##name;

class Backup : public QObject
{
    Q_OBJECT
    Q_ENUMS(Status)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
public:
    enum Status {
        Stopped,
        Waiting,
        Running,
        Restoring
    };

    explicit Backup(QObject *parent = 0);
    ~Backup();

    Status status() const;
    Q_INVOKABLE QVariantList jobList() const;
    bool enabled() const;
    Q_INVOKABLE void backup(const QString &name);
    Q_INVOKABLE void restore(int fileTreeId, const QString &to = QString());
    Q_INVOKABLE QVariantMap contents(const QString &path, const QString &backup = QString()) const;
    Q_INVOKABLE QVariantMap diff(const QString &path, const QString &backup) const;
    Q_INVOKABLE QVariantList tree() const;
    Q_INVOKABLE QVariantList excludePatterns() const;
    Q_INVOKABLE void addExcludePattern(const QString &pattern, bool entireDirectory);
    Q_INVOKABLE void removeExcludePattern(const QString &id, const QString &pattern);

    void setEnabled(bool enabled);

signals:
    void done(bool ok, const QString &errorMessage);
    void enabledChanged(bool enabled);
    void statusChanged(Status status);

private:
    class Private;
    Private *d;
    QThread thread;
};

#endif // BACKUP_H

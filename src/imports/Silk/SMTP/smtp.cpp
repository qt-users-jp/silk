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

#include "smtp.h"

#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QSslSocket>

class Smtp::Private : public QObject
{
    Q_OBJECT
public:
    enum Status {
        Init,
        Ehlo,
        StartTls,
        Auth,
        From,
        Rcpt,
        Data,
        Body,
        Quit,
        Done
    };

    Private(Smtp *parent);
    void send(const QVariantMap &email);
    void send(QSslSocket *socket, const QString &command);

private slots:
    void encrypted();
    void readyRead();
    void disconnected();
    void error(QAbstractSocket::SocketError error);
    void sslErrors(QList<QSslError> errors);

private:
    void greetingReceived(QSslSocket *socket);
    void sendEhlo(QSslSocket *socket);
    void ehloReceived(QSslSocket *socket);
    void sendStartTls(QSslSocket *socket);
    void startTlsReceived(QSslSocket *socket);
    void sendAuth(QSslSocket *socket);
    void authReceived(QSslSocket *socket);
    void sendFrom(QSslSocket *socket);
    void fromReceived(QSslSocket *socket);
    void sendRcpt(QSslSocket *socket);
    void rcptReceived(QSslSocket *socket);
    void sendData(QSslSocket *socket);
    void dataReceived(QSslSocket *socket);
    void sendBody(QSslSocket *socket);
    void bodyReceived(QSslSocket *socket);
    void sendQuit(QSslSocket *socket);
    void quitReceived(QSslSocket *socket);

private:
    Smtp *q;
    Status status;
    QMap<QObject *, QVariantMap> emails;
    QMap<QObject *, QStringList> capabilities;
    QMap<QObject *, QStringList> queue;
};

Smtp::Private::Private(Smtp *parent)
    : QObject(parent)
    , q(parent)
    , status(Init)
{
}

void Smtp::Private::send(QSslSocket *socket, const QString &command)
{
    qDebug() << Q_FUNC_INFO << __LINE__ << command;
    socket->write(command.toUtf8());
}

void Smtp::Private::send(const QVariantMap &email)
{
    QSslSocket *socket = new QSslSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    QVariantMap e = email;
    e.insert("rcpt", e.value("to"));
    emails.insert(socket, e);

    qDebug() << Q_FUNC_INFO << __LINE__ << q->host() << q->port();
    if (q->m_encrypt == "SSL") {
        connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors(QList<QSslError>)));
        socket->connectToHostEncrypted(q->host(), q->port());
    } else if (q->m_encrypt == "TLS") {
        connect(socket, SIGNAL(encrypted()), this, SLOT(encrypted()));
        connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors(QList<QSslError>)));
        socket->connectToHost(q->host(), q->port());
    } else {
        socket->connectToHost(q->host(), q->port());
    }
}

void Smtp::Private::readyRead()
{
    QSslSocket *socket = qobject_cast<QSslSocket *>(sender());
    if (!socket->canReadLine()) return;

    switch (status) {
    case Init:
        greetingReceived(socket);
        break;
    case Ehlo:
        ehloReceived(socket);
        break;
    case StartTls:
        startTlsReceived(socket);
        break;
    case Auth:
        authReceived(socket);
        break;
    case From:
        fromReceived(socket);
        break;
    case Rcpt:
        rcptReceived(socket);
        break;
    case Data:
        dataReceived(socket);
        break;
    case Body:
        bodyReceived(socket);
        break;
    case Quit:
        quitReceived(socket);
        break;
    default:
        qDebug() << Q_FUNC_INFO << __LINE__ << status << socket->readAll();
        socket->disconnectFromHost();
        break;
    }
}

void Smtp::Private::greetingReceived(QSslSocket *socket)
{
    QString line = socket->readLine();
    int code = line.left(3).toInt();
    qDebug() << Q_FUNC_INFO << __LINE__ << code;
    switch (code) {
    case 220: // Greeting
        sendEhlo(socket);
        break;
    default:
        qDebug() << Q_FUNC_INFO << __LINE__ << code << line;
        emit q->error(emails.take(socket), line);
        socket->disconnectFromHost();
        break;
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::sendEhlo(QSslSocket *socket)
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    QString hostInfo(QHostInfo::localDomainName());
    if (hostInfo.isEmpty()) {
        QList<QHostAddress> addresses(QNetworkInterface::allAddresses());
        if (!addresses.isEmpty())
            hostInfo = QString("[%1]").arg(addresses.first().toString());
        else
            hostInfo = QStringLiteral("localhost.localdomain");
    }
    qDebug() << Q_FUNC_INFO << __LINE__ << hostInfo;
    send(socket, QString("EHLO %1\r\n").arg(hostInfo));
    status = Ehlo;
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::ehloReceived(QSslSocket *socket)
{
    QString line = socket->readLine();
    int code = line.left(3).toInt();
    qDebug() << Q_FUNC_INFO << __LINE__ << code;
    QStringList capability;
    switch (code) {
    case 250: // Ok
        while (socket->canReadLine()) {
            QString l = socket->readLine().mid(4);
            l.chop(2);
            capability.append(l);
        }
        capabilities.insert(socket, capability);
        qDebug() << capability;
        if (capability.contains("STARTTLS") && q->m_encrypt == QStringLiteral("TLS")) {
            sendStartTls(socket);
        } else {
            sendAuth(socket);
        }
        break;
    default:
        emit q->error(emails.take(socket), QString("EHLO: code %1 is not supported.").arg(code));
        socket->disconnectFromHost();
        break;
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::sendStartTls(QSslSocket *socket)
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    send(socket, QString("STARTTLS\r\n"));
    status = StartTls;
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::startTlsReceived(QSslSocket *socket)
{
    QString line = socket->readLine();
    int code = line.left(3).toInt();
    qDebug() << Q_FUNC_INFO << __LINE__ << code;
    switch (code) {
    case 220: // Ok
        socket->startClientEncryption();
        break;
    default:
        emit q->error(emails.take(socket), QString("STARTTLS: code %1 is not supported.").arg(code));
        socket->disconnectFromHost();
        break;
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::sendAuth(QSslSocket *socket)
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    QStringList capability = capabilities.value(socket);
    QStringList methods;
    foreach (const QString &c, capability) {
        if (c.startsWith("AUTH ")) {
            methods = c.mid(5).split(" ");
            break;
        }
    }

    qDebug() << Q_FUNC_INFO << __LINE__ << methods;
    if (methods.contains("LOGIN")) {
        QStringList cmds;
        cmds.append(QString::fromUtf8(q->m_username.toUtf8().toBase64()) + "\r\n");
        cmds.append(QString::fromUtf8(q->m_password.toUtf8().toBase64()) + "\r\n");
        queue.insert(socket, cmds);
        send(socket, QString("AUTH LOGIN\r\n"));
        status = Auth;
    } else if (methods.contains("PLAIN")) {
        QByteArray auth(q->m_username.toUtf8());
        auth.append('\0');
        auth.append(q->m_username.toUtf8());
        auth.append('\0');
        auth.append(q->m_password.toUtf8());
        send(socket, QString("AUTH PLAIN %1\r\n").arg(QString::fromUtf8(auth.toBase64())));
        status = Auth;
    } else {
        sendFrom(socket);
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::authReceived(QSslSocket *socket)
{
    QString line = socket->readLine();
    int code = line.left(3).toInt();
    qDebug() << Q_FUNC_INFO << __LINE__ << code;
    switch (code) {
    case 235: // Ok
        sendFrom(socket);
        break;
    case 334: { // Next
        QStringList cmds = queue.take(socket);
        if (!cmds.isEmpty()) {
            send(socket, cmds.takeFirst());
            queue.insert(socket, cmds);
        } else {
            emit q->error(emails.take(socket), QString("AUTH LOGIN: queue is empty."));
            socket->disconnectFromHost();
        }
        break; }
    default:
        emit q->error(emails.take(socket), QString("AUTH: code %1 is not supported.").arg(code));
        socket->disconnectFromHost();
        break;
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::sendFrom(QSslSocket *socket)
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    send(socket, QString("MAIL FROM: <%1>\r\n").arg(q->m_from));
    status = From;
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::fromReceived(QSslSocket *socket)
{
    QString line = socket->readLine();
    int code = line.left(3).toInt();
    qDebug() << Q_FUNC_INFO << __LINE__ << code;
    switch (code) {
    case 250: // Ok
        sendRcpt(socket);
        break;
    default:
        emit q->error(emails.take(socket), QString("MAIL FROM: code %1 is not supported.").arg(code));
        socket->disconnectFromHost();
        break;
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::sendRcpt(QSslSocket *socket)
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    QVariantMap email = emails.value(socket);
    QVariantList rcpt = email.value("rcpt").toList();
    qDebug() << email << rcpt;
    send(socket, QString("RCPT TO: <%1>\r\n").arg(rcpt.takeFirst().toString()));
    if (rcpt.isEmpty()) {
        status = Rcpt;
    }
    email.insert("rcpt", rcpt);
    emails.insert(socket, email);
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::rcptReceived(QSslSocket *socket)
{
    QString line = socket->readLine();
    int code = line.left(3).toInt();
    qDebug() << Q_FUNC_INFO << __LINE__ << code;
    switch (code) {
    case 250: // Ok
        sendData(socket);
        break;
    default:
        qDebug() << Q_FUNC_INFO << __LINE__ << code << line;
        emit q->error(emails.take(socket), QString("RCPT TO: code %1 is not supported.").arg(code));
        socket->disconnectFromHost();
        break;
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::sendData(QSslSocket *socket)
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    send(socket, QString("DATA\r\n"));
    status = Data;
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::dataReceived(QSslSocket *socket)
{
    QString line = socket->readLine();
    int code = line.left(3).toInt();
    qDebug() << Q_FUNC_INFO << __LINE__ << code;
    switch (code) {
    case 354: // Ok
        sendBody(socket);
        break;
    default:
        qDebug() << Q_FUNC_INFO << __LINE__ << code << line;
        emit q->error(emails.take(socket), QString("DATA: code %1 is not supported.").arg(code));
        socket->disconnectFromHost();
        break;
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::sendBody(QSslSocket *socket)
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    QVariantMap email = emails.value(socket);

    // From
    send(socket, QString("From: %1\r\n").arg(q->m_from));

    // To
    QStringList to;
    foreach (const QVariant &t, email.value("to").toList())
        to.append(t.toString());
    send(socket, QString("To: %1\r\n").arg(to.join(",")));

    // Subject
    if (email.contains("subject")) {
        send(socket, QString("Subject: %1\r\n").arg(email.value("subject").toString()));
    }

    // Body
    if (email.contains("body")) {
        QStringList body;
        body.append(QString(""));
        foreach (const QString &s, email.value("body").toString().split("\n"))
            if (s.startsWith("."))
                body.append(QString(".%1").arg(s));
            else
                body.append(s);
        body.append(QString(""));
        send(socket, body.join("\r\n"));
    }

    // Terminate
    send(socket, QString(".\r\n"));
    status = Body;
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::bodyReceived(QSslSocket *socket)
{
    QString line = socket->readLine();
    int code = line.left(3).toInt();
    qDebug() << Q_FUNC_INFO << __LINE__ << code;
    switch (code) {
    case 250: // Ok
        sendQuit(socket);
        break;
    default:
        qDebug() << Q_FUNC_INFO << __LINE__ << code << line;
        emit q->error(emails.take(socket), QString("BODY: code %1 is not supported.").arg(code));
        socket->disconnectFromHost();
        break;
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::sendQuit(QSslSocket *socket)
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    send(socket, QString("QUIT\r\n"));
    status = Quit;
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::quitReceived(QSslSocket *socket)
{
    QString line = socket->readLine();
    int code = line.left(3).toInt();
    qDebug() << Q_FUNC_INFO << __LINE__ << code;
    switch (code) {
    case 221: // Ok
        socket->disconnectFromHost();
        emit q->sent(emails.take(socket));
        status = Done;
        break;
    default:
        qDebug() << Q_FUNC_INFO << __LINE__ << code << line;
        emit q->error(emails.take(socket), QString("QUIT: code %1 is not supported.").arg(code));
        socket->disconnectFromHost();
        break;
    }
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::encrypted()
{
    QSslSocket *socket = qobject_cast<QSslSocket *>(sender());
    sendEhlo(socket);
//    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::disconnected()
{
    QSslSocket *socket = qobject_cast<QSslSocket *>(sender());
    if (emails.contains(socket)) {
        emails.take(socket);
    }
    if (capabilities.contains(socket)) {
        capabilities.take(socket);
    }
    if (queue.contains(socket)) {
        queue.take(socket);
    }
    socket->deleteLater();
    qDebug() << Q_FUNC_INFO << __LINE__;
}

void Smtp::Private::error(QAbstractSocket::SocketError error)
{
    QSslSocket *socket = qobject_cast<QSslSocket *>(sender());
    qDebug() << Q_FUNC_INFO << __LINE__ << error;
    socket->disconnectFromHost();
}

void Smtp::Private::sslErrors(QList<QSslError> errors)
{
    QSslSocket *socket = qobject_cast<QSslSocket *>(sender());
    qDebug() << Q_FUNC_INFO << __LINE__ << errors;
//    socket->disconnectFromHost();
    socket->ignoreSslErrors();
}

Smtp::Smtp(QObject *parent)
    : QObject(parent)
{
}

void Smtp::send(const QVariantMap &email)
{
    Private *d = new Private(this);
    d->send(email);
}

#include "smtp.moc"


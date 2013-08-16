#include "silkconfig.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>

QVariantMap SilkConfig::m_config;
QString SilkConfig::m_file;

const QVariantMap &SilkConfig::config()
{
    return m_config;
}

QVariant SilkConfig::value(const QVariant &v, const QString &key)
{
    int index = key.indexOf(QLatin1Char('.'));
    if (index > 0) {
        return value(v.toMap().value(key.left(index)), key.mid(index + 1));
    }
    return v.toMap().value(key);
}

QVariant SilkConfig::value(const QString &key)
{
    int index = key.indexOf(QLatin1Char('.'));
    if (index > 0) {
        QVariant ret = value(m_config.value(key.left(index)), key.mid(index + 1));
        return ret;
    }
    return m_config.value(key);
}

void SilkConfig::initialize(int argc, char **argv)
{
    m_config = readConfigFile(QString(":/%1rc").arg(QCoreApplication::instance()->applicationName().toLower()));
    QString fileName = QDir::home().absoluteFilePath(QString(".%1rc").arg(QCoreApplication::instance()->applicationName()));
    bool userConfig = false;
    for (int i = 1; i < argc; i++) {
        if (QString::fromUtf8(argv[i]) == QStringLiteral("--config")) {
            if (argc - i > 1) {
                fileName = argv[++i];
                userConfig = true;
                break;
            }
        }
    }

    if (QFile::exists(fileName)) {
        QVariantMap override = readConfigFile(fileName);
        foreach (const QString &key, override.keys()) {
            if (m_config.contains(key)) {
                if (override.value(key).isNull()) {
                    qWarning() << "Configuration:" << key << "is ignored because of no value.";
                    qWarning() << "Expected:" << m_config.value(key);
                    continue;
                }
                if (m_config.value(key).type() != override.value(key).type()) {
                    qWarning() << "Configuration:" << key << "is ignored because of type mismatch.";
                    qWarning() << "Expected:" << m_config.value(key).type() << "Actual:" << override.value(key).type();
                    continue;
                }
            }
            m_config.insert(key, override.value(key));
        }
    } else if (userConfig){
        qWarning() << fileName << "not found.";
    }
}

QVariantMap SilkConfig::readConfigFile(const QString &fileName)
{
    m_file = fileName;
    QVariantMap ret;
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QJsonParseError error;
        QByteArray data = file.readAll();
        file.close();

        data = data.replace(QStringLiteral("$${SILK_DATA_PATH}"), SILK_DATA_PATH);

        QJsonDocument doc = QJsonDocument::fromJson(data, &error);
        if (error.error == QJsonParseError::NoError) {
            ret = doc.toVariant().toMap();
        } else {
            qWarning() << error.offset << error.errorString();
        }
    } else {
        qWarning() << file.error() << file.errorString();
    }
    return ret;
}

const QString &SilkConfig::file()
{
    return m_file;
}

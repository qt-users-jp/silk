#include "silkconfig.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>

QVariantMap SilkConfig::config;

QVariant SilkConfig::value(const QString &key)
{
    return config.value(key);
}

void SilkConfig::initialize(int argc, char **argv)
{
    config = readConfigFile(":/silkrc.default");
    QString fileName = QDir::home().absoluteFilePath(".silkrc");
    for (int i = 1; i < argc; i++) {
        if (QString::fromUtf8(argv[i]) == QLatin1String("--config")) {
            if (argc - i > 1) {
                fileName = argv[++i];
                break;
            }
        }
    }

    if (QFile::exists(fileName)) {
        QVariantMap override = readConfigFile(fileName);
        foreach (const QString &key, override.keys()) {
            if (!config.contains(key)) {
                qWarning() << "Configuration:" << key << "is ignored because of invalid key.";
                continue;
            }
            if (override.value(key).isNull()) {
                qWarning() << "Configuration:" << key << "is ignored because of no value.";
                qWarning() << "Expected:" << config.value(key);
                continue;
            }
            if (config.value(key).type() != override.value(key).type()) {
                qWarning() << "Configuration:" << key << "is ignored because of type mismatch.";
                qWarning() << "Expected:" << config.value(key).type() << "Actual:" << override.value(key).type();
                continue;
            }
            config.insert(key, override.value(key));
        }
    } else {
        qWarning() << fileName;
    }
}

QVariantMap SilkConfig::readConfigFile(const QString &fileName)
{
    QVariantMap ret;
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
        file.close();
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

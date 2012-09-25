#include "silkconfig.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>

QVariantMap SilkConfig::config;

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
        QVariant ret = value(config.value(key.left(index)), key.mid(index + 1));
        return ret;
    }
    return config.value(key);
}

void SilkConfig::initialize(int argc, char **argv)
{
    config = readConfigFile(":/silkrc.default");
    QString fileName = QDir::home().absoluteFilePath(".silkrc");
    bool userConfig = false;
    for (int i = 1; i < argc; i++) {
        if (QString::fromUtf8(argv[i]) == QLatin1String("--config")) {
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
    } else if (userConfig){
        qWarning() << fileName << "not found.";
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

#include "silkconfig.h"

#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>

QVariantHash SilkConfig::m_config;
QUrl SilkConfig::m_url;

class SilkServerConfigListen : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString address MEMBER address NOTIFY addressChanged)
    Q_PROPERTY(int port MEMBER port NOTIFY portChanged)
public:
    explicit SilkServerConfigListen(QObject *parent = 0) : QObject(parent), address(QStringLiteral("localhost")), port(8080) {}

signals:
    void addressChanged(const QString &address);
    void portChanged(int port);

private:
    QString address;
    int port;
};

class SilkServerConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SilkServerConfigListen *listen MEMBER listen NOTIFY listenChanged)
    Q_PROPERTY(QJsonObject contents MEMBER contents NOTIFY contentsChanged)
    Q_PROPERTY(QJsonArray daemons MEMBER daemons NOTIFY daemonsChanged)
    Q_PROPERTY(QJsonArray imports MEMBER imports NOTIFY importsChanged)
    Q_PROPERTY(QString offlineStoragePath MEMBER offlineStoragePath NOTIFY offlineStoragePathChanged)
    Q_PROPERTY(QJsonObject cache MEMBER cache NOTIFY cacheChanged)
    Q_PROPERTY(QJsonObject deflate MEMBER deflate NOTIFY deflateChanged)
    Q_PROPERTY(QJsonObject rewrite MEMBER rewrite NOTIFY rewriteChanged)
public:
    explicit SilkServerConfig(QObject *parent = 0) : QObject(parent) {}

signals:
    void listenChanged(const SilkServerConfigListen &listen);
    void contentsChanged(const QJsonObject &contents);
    void daemonsChanged(const QJsonArray &daemons);
    void importsChanged(const QJsonArray &imports);
    void offlineStoragePathChanged(const QString &offlineStoragePath);
    void cacheChanged(const QJsonObject &cache);
    void deflateChanged(const QJsonObject &deflate);
    void rewriteChanged(const QJsonObject &rewrite);

private:
    SilkServerConfigListen *listen;
    QJsonObject contents;
    QJsonArray daemons;
    QJsonArray imports;
    QString offlineStoragePath;
    QJsonObject cache;
    QJsonObject deflate;
    QJsonObject rewrite;
};

const QVariantHash &SilkConfig::config()
{
    return m_config;
}

QVariant SilkConfig::value(const QVariant &v, const QString &key)
{
    QVariantHash hash;
    switch (v.type()) {
    case QVariant::Hash:
        hash = v.toHash();
        break;
    case QVariant::Map: {
        QMap<QString, QVariant> map = v.toMap();
        foreach (const QString &k, map.keys())
            hash.insert(k, map.value(k));
        break; }
    default:
        qWarning() << v << "not supported";
        break;
    }

    int index = key.indexOf(QLatin1Char('.'));
    if (index > 0) {
        if(!hash.contains(key.left(index))) {
            qWarning() << "No config value found for" << key.left(index);
            return QVariant();
        }
        return value(hash.value(key.left(index)), key.mid(index + 1));
    }
    if (!hash.contains(key)) {
        qWarning() << "No config value found for" << key;
        return QVariant();
    }
    return hash.value(key);
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

void SilkConfig::initialize(QCoreApplication *app)
{
    m_config = readConfigFile(QString("qrc:/%1config.qml").arg(QCoreApplication::instance()->applicationName().toLower()));
    QString fileName = QDir::home().absoluteFilePath(QString(".%1config.qml").arg(QCoreApplication::instance()->applicationName()));

    QCommandLineParser commandLine;
    QCommandLineOption config(QStringLiteral("config"), QStringLiteral("configuration qml file"), QStringLiteral("silkconfig.qml"));
    commandLine.addOption(config);
    QCommandLineOption address(QStringLiteral("listen.address"), QStringLiteral("listening address"), QStringLiteral("localhost"));
    commandLine.addOption(address);
    QCommandLineOption port(QStringLiteral("listen.port"), QStringLiteral("listening port"), QStringLiteral("8080"));
    commandLine.addOption(port);
    commandLine.process(*app);

    if (commandLine.isSet(QStringLiteral("config"))) {
        QString userFileName = commandLine.value(QStringLiteral("config"));
        if (!QFile::exists(userFileName)) {
            qWarning() << userFileName << "not found." << QDir::currentPath();
        } else {
            fileName = userFileName;
        }
    }

    if (QFile::exists(fileName)) {
        QVariantHash override = readConfigFile(QUrl::fromLocalFile(fileName));
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
    }

    QVariantHash listen = m_config.value(QLatin1String("listen")).toHash();
    if (commandLine.isSet(QStringLiteral("listen.address"))) {
        listen.insert(QStringLiteral("address"), commandLine.value(QStringLiteral("listen.address")));
    }
    if (commandLine.isSet(QStringLiteral("listen.port"))) {
        listen.insert(QStringLiteral("port"), commandLine.value(QStringLiteral("listen.port")).toInt());
    }
    m_config.insert(QStringLiteral("listen"), listen);
}

static QVariantHash readObject(const QObject *object)
{
    QVariantHash ret;

    const QMetaObject *metaObject = object->metaObject();
    for (int i = 0; i < metaObject->propertyCount(); i++) {
        QMetaProperty property = metaObject->property(i);
        QString key = QString::fromUtf8(property.name());
        QVariant value = property.read(object);
        QObject *child = qvariant_cast<QObject *>(value);
        if (child) {
            ret.insert(key, readObject(child));
        } else {
            if (value.canConvert<QJSValue>()) {
                ret.insert(key, value.value<QJSValue>().toVariant());
            } else {
                ret.insert(key, value);
            }
        }
    }
    metaObject = &object->staticMetaObject;
    for (int i = 0; i < metaObject->propertyCount(); i++) {
        ret.remove(metaObject->property(i).name());
    }

    delete object;
    return ret;
}

QVariantHash SilkConfig::readConfigFile(const QUrl &fileName)
{
    QVariantHash ret;
    m_url = fileName;

    QQmlEngine engine;
    qmlRegisterType<SilkServerConfigListen>("Silk.Config", 0, 1, "Listen");
    qmlRegisterType<SilkServerConfig>("Silk.Config", 0, 1, "Config");
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("SILK_DATA_PATH", SILK_DATA_PATH);
    QQmlComponent component(&engine, m_url);
    QObject *object = component.create();
    ret = readObject(object);
    qmlClearTypeRegistrations();

    return ret;
}

QUrl SilkConfig::url()
{
    return m_url;
}

#include "silkconfig.moc"

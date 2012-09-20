#ifndef SILKCONFIG_H
#define SILKCONFIG_H

#include <QtCore/QVariant>

class SilkConfig
{
public:
    static void initialize(int argc, char **argv);
    static QVariant value(const QString &key);
private:
    SilkConfig() {}

    static QVariantMap readConfigFile(const QString &fileName);

    static QVariantMap config;
};

#endif // SILKCONFIG_H

#ifndef SILKCONFIG_H
#define SILKCONFIG_H

#include "silkglobal.h"

#include <QtCore/QVariant>

class SILK_EXPORT SilkConfig
{
public:
    static void initialize(int argc, char **argv);
    static QVariant value(const QString &key);
private:
    SilkConfig() {}

    static QVariantMap readConfigFile(const QString &fileName);
    static QVariant value(const QVariant &v, const QString &key);

    static QVariantMap config;
};

#endif // SILKCONFIG_H

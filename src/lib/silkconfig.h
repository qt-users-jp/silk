#ifndef SILKCONFIG_H
#define SILKCONFIG_H

#include "silkglobal.h"

#include <QtCore/QVariant>

class QCoreApplication;

class SILK_EXPORT SilkConfig
{
public:
    static void initialize(QCoreApplication *app);
    static const QVariantHash &config();
    static QVariant value(const QString &key);
    static QUrl url();

private:
    SilkConfig() {}

    static QVariantHash readConfigFile(const QUrl &url);
    static QVariant value(const QVariant &v, const QString &key);

    static QVariantHash m_config;
    static QUrl m_url;
};

#endif // SILKCONFIG_H

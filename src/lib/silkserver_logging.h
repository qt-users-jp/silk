#ifndef SILKSERVER_LOGGING_H
#define SILKSERVER_LOGGING_H

#include <QtCore/QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(silkServerDebug)
Q_DECLARE_LOGGING_CATEGORY(silkServerRequest)
Q_DECLARE_LOGGING_CATEGORY(silkServerOk)
Q_DECLARE_LOGGING_CATEGORY(silkServerError)

#define ssDebug() qCDebug(silkServerDebug)
#define ssWarning() qCWarning(silkServerDebug)
#define ssCritical() qCCritical(silkServerDebug)
#define ssInfo() qCInfo(silkServerDebug)
#define ssFatal(...) qCFatal(silkServerDebug, __VA_ARGS__)

#define ssRequest() qCInfo(silkServerRequest)
#define ssOk() qCInfo(silkServerOk)
#define ssError() qCInfo(silkServerError)

#endif // SILKSERVER_LOGGING_H

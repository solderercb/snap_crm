#ifndef GLOBAL_NS_H
#define GLOBAL_NS_H
#include <QMetaObject>
#include <qobjectdefs.h>

namespace Global
{
    void throwError(const QSqlError &err, const QString &customErrorText = QString());
    void throwError(const int type, const QString &errorText = QString());
#ifdef QT_DEBUG
    void throwDebug();
#endif
}

#endif // GLOBAL_NS_H

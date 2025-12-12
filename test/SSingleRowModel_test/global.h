/* Это минимальный набор глобальных переменных и методов, необходимый для
 * работы с классом SSingleRowModel независимо от всего проекта
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#define DBG_VAR_OUT(var)   STR(| var:) << (var)

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QLocale>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QSettings>

namespace Global
{
enum ThrowType {TimeError,ResultError};
    void errorMsg(const QString &text);
    void errorPopupMsg(const QString &text);
    void throwError(const QSqlError &err, const QString &customErrorText = QString());
    void throwError(const int type, const QString &errorText = QString());
}

class SUserSettings {
    public:
        int id() {return 1;};
        int currentOffice() {return 1;};
    };

extern QMap<QString, QVariant> *loginCreds;
extern SUserSettings *userDbData;

#endif // GLOBAL_H

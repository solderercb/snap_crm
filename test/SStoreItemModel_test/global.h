/* Это минимальный набор глобальных переменных и методов, необходимый для
 * работы с классом SSingleRowModel независимо от всего проекта
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QLocale>
#include <QSettings>
#include <QString>
#include <QSqlError>
#include <QDebug>

namespace Global
{
enum ThrowType {TimeError,ResultError,IntegrityError};
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

extern SUserSettings *userDbData;
extern QLocale sysLocale;

#endif // GLOBAL_H

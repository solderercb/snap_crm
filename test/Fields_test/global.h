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
#include <SAppLog>

namespace Global
{
enum ThrowType {TimeError,ResultError,IntegrityError};
    void errorMsg(const QString &text);
    void errorPopupMsg(const QString &text);
    void throwError(const QSqlError &err, const QString &customErrorText = QString());
    void throwError(const int type, const QString &errorText = QString());
}

class SUserSettings : public QObject
{
    Q_OBJECT
signals:
    void rowHeightChanged();
public:
    explicit SUserSettings(){};
    int id() {return 1;};
    int currentOffice() {return 1;};
    int rowHeight() {return 16;}
};

extern QMap<QString, QVariant> *loginCreds;
extern SUserSettings *userDbData;
extern SAppLog *appLog;
extern QLocale sysLocale;
extern QString commonComboBoxStyleSheet;
extern QString commonComboBoxStyleSheetRed;
extern QString commonLineEditStyleSheet;
extern QString commonLineEditStyleSheetRed;
extern QString commonDateEditStyleSheet;
extern QString commonDateEditStyleSheetRed;

#endif // GLOBAL_H

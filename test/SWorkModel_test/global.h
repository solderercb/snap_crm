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
#include "sstandarditemmodel.h"
#include <SSqlQueryModel>
#include <SAppLog>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)   // препроцессор заключит конструкцию x в кавычки
#define DBG_VAR_OUT(var)   STR(| var:) << (var)
#define DBG_FUNC_NAME QString::fromStdString(std::string(__func__) + "()")

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
    int rowHeight() {return 16;};
};

extern SUserSettings *userDbData;
extern SSqlQueryModel *usersModel;
extern SSqlQueryModel *usersSalaryTaxesModel;
extern SAppLog *appLog;
extern QLocale sysLocale;
extern SStandardItemModel *warrantyTermsModel;
extern QString commonComboBoxStyleSheet;
extern QString commonComboBoxStyleSheetRed;
extern QString commonLineEditStyleSheet;
extern QString commonLineEditStyleSheetRed;
extern QString commonDateEditStyleSheet;
extern QString commonDateEditStyleSheetRed;

#endif // GLOBAL_H

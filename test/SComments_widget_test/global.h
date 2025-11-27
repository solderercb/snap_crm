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
#include <SUserActivityModel>
#include <SStandardItemModel>
#include <SSqlQueryModel>
#include <SAppLog>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)   // препроцессор заключит конструкцию x в кавычки
#define DBG_VAR_OUT(var)   STR(| var:) << (var)
#define DBG_FUNC_NAME QString::fromStdString(std::string(__func__) + "()")

#define APP_VER_STR "0.0.0.0"

namespace Global
{
    enum ThrowType {TimeError,ResultError,IntegrityError, InputError, ConditionsError,ConnLost,QueryError};
    enum RepStateIds{GetIn, Diag, Negotiation, Agreed, InWork, PartsRequested, Ready, ReadyNoRepair, Returned, IssueNotAppeared, OfficeChange, NotAgreed, ReturnedNoRepair, OnApprovement, Approved, DiagFinished, ReturnedInCredit};
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
    void fontSizeChanged();
public:
    explicit SUserSettings(){};
    int id() {return 1;};
    int company() {return 1;};
    int currentOffice() {return 1;};
    int rowHeight() {return 16;};
    int fontSize(){return 14;};
    void updateActivityTimestamp(){};
};

class SComSettings
{
public:
    bool classicKassa() {return 0;};
};

class SPermissions
{
public:
    bool editAnyComment = 1;
};

extern SAppLog *appLog;
extern SUserActivityModel *userActivityLog;
extern QLocale sysLocale;
extern SComSettings *comSettings;
extern SSqlQueryModel *companiesModel;
extern SUserSettings *userDbData;
extern SSqlQueryModel *usersModel;
extern QMap<int, QString> *allUsersMap;
extern SPermissions *permissions;
extern SStandardItemModel *warrantyTermsModel;

#endif // GLOBAL_H

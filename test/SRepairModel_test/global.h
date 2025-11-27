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
#include <SStandardItemModel>
#include <SSqlQueryModel>
#include <SAppLog>
#include <SComSettings>

#define DBG_VAR_OUT(var)   STR(| var:) << (var)
#define DBG_FUNC_NAME QString::fromStdString(std::string(__func__) + "()")

namespace Global
{
    enum ThrowType {TimeError,ResultError,IntegrityError, InputError, ConditionsError,ConnLost};
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
public:
    explicit SUserSettings(){};
    int id() {return m_id;};
    void set_id(const int id) {m_id = id;};
    int company() {return 1;};
    int currentOffice() {return 1;};
    int rowHeight() {return 16;};
private:
    int m_id = 1;
};

class SPermissions
{
public:
    bool viewClients = 1;
};

extern SComSettings *comSettings;
extern SUserSettings *userDbData;
extern SSqlQueryModel *usersModel;
extern SSqlQueryModel *allUsersModel;
extern SSqlQueryModel *companiesModel;
extern SSqlQueryModel *officesModel;
extern SSqlQueryModel *managersModel;
extern SSqlQueryModel *engineersModel;
extern SAppLog *appLog;
extern QLocale sysLocale;
extern SPermissions *permissions;
extern SSqlQueryModel *paymentSystemsModel;
extern SSqlQueryModel *repairBoxesModel;
extern SStandardItemModel *warrantyTermsModel;
extern QMap<int, QString> *warrantyTermsMap;
extern SStandardItemModel *notifyStatusesModel;

#endif // GLOBAL_H

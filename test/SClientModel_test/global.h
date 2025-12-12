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

namespace Global
{
enum ThrowType {TimeError,ResultError,IntegrityError, InputError};
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

class SComSettings
{
public:
    bool isClientPatronymicRequired() {return 1;};
    bool isVisitSourceRequired() {return 0;};
    bool isClientPhoneRequired() {return 1;};
    bool printWorksList() {return 1;};
    bool newClientSmsEnabled() {return 1;};
    QString currencyId() {return "USD";};
};

class SPermissions
{
public:
    bool viewClients = 1;
    bool viewClientPassportData = 1;
};

extern QMap<QString, QVariant> *loginCreds;
extern SComSettings *comSettings;
extern SUserSettings *userDbData;
extern SSqlQueryModel *usersModel;
extern SSqlQueryModel *officesModel;
extern SAppLog *appLog;
extern QLocale sysLocale;
extern SPermissions *permissions;
extern SStandardItemModel *clientBinaryProperties;
extern SSqlQueryModel* clientAdTypesList;
extern SStandardItemModel *priceColModel;
extern SSqlQueryModel *clientPhoneTypesModel;

#endif // GLOBAL_H

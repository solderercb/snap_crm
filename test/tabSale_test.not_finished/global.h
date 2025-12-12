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
#include <SSqlQueryModel>
#include <SStandardItemModel>
#include "../../clients4test.h"

namespace Global
{
enum ThrowType {TimeError,ResultError,IntegrityError, UserCanceled};
    void errorMsg(const QString &text);
    void errorPopupMsg(const QString &text);
    void throwError(const QSqlError &err, const QString &customErrorText = QString());
    void throwError(const int type, const QString &errorText = QString());
    enum Reports {pko = 2, rn = 13};
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
    int fontSize() {return 14;};
};

class SComSettings
{
public:
    bool printOutInvoice() {return 0;};
    bool printPKO() {return 0;};
    int defaultItemReserveTime() {return 5;};
    bool isClientPatronymicRequired() {return 1;};
    bool isVisitSourceRequired() {return 0;};
    bool isClientPhoneRequired() {return 1;};
};

class SPermissions
{
public:
    bool viewClients = 1;
    bool undoOutInvoice = 1;
};

extern QMap<QString, QVariant> *loginCreds;
extern SUserSettings *userDbData;
extern SAppLog *appLog;
extern QLocale sysLocale;
extern SComSettings *comSettings;
extern SPermissions *permissions;
extern SSqlQueryModel *companiesModel;
extern SSqlQueryModel *paymentSystemsModel;
extern SSqlQueryModel *clientPhoneTypesModel;
extern SSqlQueryModel* clientAdTypesList;
extern SStandardItemModel *priceColModel;
extern QString commonComboBoxStyleSheet;
extern QString commonComboBoxStyleSheetRed;
extern QString commonLineEditStyleSheet;
extern QString commonLineEditStyleSheetRed;
extern QString commonDateEditStyleSheet;
extern QString commonDateEditStyleSheetRed;



#endif // GLOBAL_H

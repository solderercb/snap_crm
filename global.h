#ifndef GLOBAL_H
#define GLOBAL_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QLocale>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "applog.h"
#include "models/suseractivitymodel.h"
#include "models/sstandarditemmodel.h"
#include "models/ssqlquerymodel.h"
#include "models/spaymenttypesmodel.h"
#ifdef QT_DEBUG
#include "clients4test.h"
#endif

namespace Global
{
    Q_NAMESPACE
    enum RepStateHeaders{Name, Id, Color, Terms, Contains, Actions, Roles};
    Q_ENUM_NS(RepStateHeaders)
    enum RepStateIds{GetIn, Diag, Negotiation, Agreed, InWork, PartsRequested, Ready, ReadyNoRepair, Returned, IssueNotAppeared, OfficeChange, NotAgreed, ReturnedNoRepair, OnApprovement, Approved, DiagFinished, ReturnedInCredit};
    Q_ENUM_NS(RepStateIds)
    enum RepStateActions{EditWorksParts = 1, ResetInformedStatus = 2, EditDiagSumm = 3, InformManager = 4, InformEngineer = 5, InformTimeout = 6, NoPayDiag = 7}; // значения больше 6 - мои доработки
    Q_ENUM_NS(RepStateActions)
    enum PaymentSystemIds{Balance = -2, Card = -1, Cash = 0, Cashless = 1};
    Q_ENUM_NS(PaymentSystemIds)
}

extern QLocale sysLocale;
extern QVector<QSqlDatabase *> connections;    // массив указателей на соединения (для установки всем соединениям одинаковых параметров)
extern QMap<QString, QVariant> *userDbData;
extern QSqlQueryModel *userDbDataModel;
extern QMap<QString, QVariant> *userLocalData;
extern QMap<QString, bool> *permissions;
extern QMap<QString, QVariant> *comSettings;
//extern SStandardItemModel *clientPhoneTypesModel;
extern SSqlQueryModel *clientPhoneTypesModel;
extern SSqlQueryModel *companiesModel;
extern SSqlQueryModel *officesModel;
extern QSqlQueryModel *warehousesModel;
extern SSqlQueryModel *allUsersModel;
extern QMap<int, QString> *allUsersMap;
extern SSqlQueryModel *usersModel;
extern SSqlQueryModel *usersSalaryTaxesModel;
extern SSqlQueryModel *managersModel;
extern SSqlQueryModel *engineersModel;
extern SSqlQueryModel *itemBoxesModel;
extern SSqlQueryModel *repairBoxesModel;
extern SSqlQueryModel *paymentSystemsModel;
extern SPaymentTypesModel *receiptTypesModel;
extern SPaymentTypesModel *expenditureTypesModel;
extern QStandardItemModel* clientsTypesList;
extern SSqlQueryModel* clientAdTypesList;
extern SStandardItemModel *statusesModel;
extern SStandardItemModel *notifyStatusesModel;
extern SStandardItemModel *warrantyTermsModel;
extern QMap<int, QString> *warrantyTermsMap;
extern QString commonComboBoxStyleSheet;
extern QString commonComboBoxStyleSheetRed;
extern QString commonLineEditStyleSheet;
extern QString commonLineEditStyleSheetRed;
extern QString commonTextEditStyleSheet;
extern QString commonTextEditStyleSheetRed;
extern QString commonDateEditStyleSheet;
extern QString commonDateEditStyleSheetRed;
extern QString commonSpinBoxStyleSheet;
extern QString commonSpinBoxStyleSheetRed;
extern SStandardItemModel *rejectReasonModel;
extern SStandardItemModel *priceColModel;
extern SStandardItemModel *itemUnitsModel;
extern SAppLog *appLog;
extern SUserActivityModel *userActivityLog;
extern SStandardItemModel *clientBinaryProperties;

//extern QSqlQueryModel *Model;
//extern QSqlQueryModel *Model;

bool readStatuses(QStandardItemModel &, QJsonArray &);
QString jsonArrayJoin(QJsonValue, const QString);
void initSystemObjects();
void initUserDbData();
void initPermissions();
void initCompanies();
void initOffices();
void initGlobalModels();    // общие модели данных: организации, офисы, склады, сотрудники (все, менеджеры, инженеры) и др.

#endif // GLOBAL_H

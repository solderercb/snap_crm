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
#include "models/sstandarditemmodel.h"
#include "models/ssqlquerymodel.h"
#ifdef QT_DEBUG
#include "clients4test.h"
#endif

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
extern SSqlQueryModel *managersModel;
extern SSqlQueryModel *engineersModel;
extern SSqlQueryModel *itemBoxesModel;
extern SSqlQueryModel *repairBoxesModel;
extern SSqlQueryModel *paymentSystemsModel;
extern QStandardItemModel* clientsTypesList;
extern SSqlQueryModel* clientAdTypesList;
extern QStandardItemModel *statusesModel;
extern QStandardItemModel *notifyStatusesModel;
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
extern SStandardItemModel *rejectReasonModel;
extern SStandardItemModel *priceColModel;
extern SStandardItemModel *itemUnitsModel;
extern SAppLog *appLog;

//extern QSqlQueryModel *Model;
//extern QSqlQueryModel *Model;

bool readStatuses(QStandardItemModel &, QJsonArray &);
QString jsonArrayJoin(QJsonValue, const QString);
void initUserDbData();
void initPermissions();
void initCompanies();
void initOffices();
void initGlobalModels();    // общие модели данных: организации, офисы, склады, сотрудники (все, менеджеры, инженеры) и др.

#endif // GLOBAL_H

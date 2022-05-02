#ifndef GLOBAL_H
#define GLOBAL_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QLocale>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "sstandarditemmodel.h"
#include "ssqlquerymodel.h"
#ifdef QT_DEBUG
#include "clients4test.h"
#endif

extern QLocale sysLocale;
extern QVector<QSqlDatabase *> connections;    // массив указателей на соединения (для установки всем соединениям одинаковых параметров)
extern QMap<QString, QVariant> *userData;
extern QSqlQueryModel *userDataModel;
extern QMap<QString, QVariant> *userLocalData;
extern QMap<QString, bool> *permissions;
extern QMap<QString, QVariant> *comSettings;
extern SStandardItemModel *clientPhoneTypesModel;
extern SSqlQueryModel *companiesModel;
extern QSqlQueryModel *officesModel;
extern QSqlQueryModel *warehousesModel;
extern QSqlQueryModel *allUsersModel;
extern QMap<int, QString> *allUsersMap;
extern QSqlQueryModel *usersModel;
extern QSqlQueryModel *managersModel;
extern QSqlQueryModel *engineersModel;
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

//extern QSqlQueryModel *Model;
//extern QSqlQueryModel *Model;

bool readStatuses(QStandardItemModel &, QJsonArray &);
QString jsonArrayJoin(QJsonValue, const QString);
void initUserData();
void initPermissions();
void initCompanies();
void initOffices();
void initGlobalModels();    // общие модели данных: организации, офисы, склады, сотрудники (все, менеджеры, инженеры) и др.

#endif // GLOBAL_H

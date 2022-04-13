#ifndef GLOBAL_H
#define GLOBAL_H

#include <QLocale>
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

#endif // GLOBAL_H

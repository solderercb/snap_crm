#include "global.h"
QMap<QString, QVariant> *userData = new QMap<QString, QVariant>;
QSqlQueryModel *userDataModel = new QSqlQueryModel();
QMap<QString, QVariant> *userLocalData = new QMap<QString, QVariant>;
QMap<QString, bool> *permissions = new QMap<QString, bool>;
QMap<QString, QVariant> *comSettings = new QMap<QString, QVariant>;
QSqlQueryModel *companiesModel = new QSqlQueryModel;
QSqlQueryModel *officesModel = new QSqlQueryModel;
QSqlQueryModel *warehousesModel = new QSqlQueryModel;
QSqlQueryModel *usersModel = new QSqlQueryModel;
QSqlQueryModel *managersModel = new QSqlQueryModel;
QSqlQueryModel *engineersModel = new QSqlQueryModel;
QSqlQueryModel *itemBoxesModel = new QSqlQueryModel;
QSqlQueryModel *repairBoxesModel = new QSqlQueryModel;
QSqlQueryModel *paymentSystemsModel = new QSqlQueryModel;


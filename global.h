#ifndef GLOBAL_H
#define GLOBAL_H

extern QMap<QString, QVariant> *userData;
extern QMap<QString, QVariant> *userLocalData;
extern QMap<QString, bool> *permissions;
extern QMap<QString, QVariant> *comSettings;
extern QSqlQueryModel *companiesModel;
extern QSqlQueryModel *officesModel;
extern QSqlQueryModel *warehousesModel;
extern QSqlQueryModel *usersModel;
extern QSqlQueryModel *managersModel;
extern QSqlQueryModel *engineersModel;
extern QSqlQueryModel *itemBoxesModel;
extern QSqlQueryModel *repairBoxesModel;
extern QSqlQueryModel *paymentSystemsModel;
//extern QSqlQueryModel *Model;
//extern QSqlQueryModel *Model;

#endif // GLOBAL_H

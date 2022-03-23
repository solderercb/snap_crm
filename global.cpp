#include "global.h"
QVector<QSqlDatabase *> connections;    // массив указателей на соединения (для установки всем соединениям одинаковых параметров)
QMap<QString, QVariant> *userData = new QMap<QString, QVariant>;
QSqlQueryModel *userDataModel = new QSqlQueryModel();
QMap<QString, QVariant> *userLocalData = new QMap<QString, QVariant>;
QMap<QString, bool> *permissions = new QMap<QString, bool>;
QMap<QString, QVariant> *comSettings = new QMap<QString, QVariant>;
QSqlQueryModel *companiesModel = new QSqlQueryModel;
QSqlQueryModel *officesModel = new QSqlQueryModel;
QSqlQueryModel *warehousesModel = new QSqlQueryModel;
QSqlQueryModel *allUsersModel = new QSqlQueryModel;
QMap<int, QString> *allUsersMap = new QMap<int, QString>;
QSqlQueryModel *usersModel = new QSqlQueryModel;
QSqlQueryModel *managersModel = new QSqlQueryModel;
QSqlQueryModel *engineersModel = new QSqlQueryModel;
QSqlQueryModel *itemBoxesModel = new QSqlQueryModel;
QSqlQueryModel *repairBoxesModel = new QSqlQueryModel;
QSqlQueryModel *paymentSystemsModel = new QSqlQueryModel;
QStandardItemModel* clientsTypesList = new QStandardItemModel;
QSqlQueryModel* clientAdTypesList = new QSqlQueryModel;
QStandardItemModel *statusesModel = new QStandardItemModel();
QStandardItemModel *notifyStatusesModel = new QStandardItemModel();
QStandardItemModel *warrantyTermsModel = new QStandardItemModel();
QMap<int, QString> *warrantyTermsMap = new QMap<int, QString>;
QString commonComboBoxStyleSheet = "QComboBox {  border: 1px solid gray;  padding: 1px 18px 1px 3px;}\
        QComboBox::drop-down {  border: 0px;}\
        QComboBox::down-arrow{  image: url(down-arrow.png);  width: 16px;  height: 20px;}\
        QComboBox::hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}\
        QComboBox::down-arrow:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}";

//QWidget *modalWidget = nullptr;



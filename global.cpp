#include "global.h"

QLocale sysLocale = QLocale::system();
QVector<QSqlDatabase *> connections;    // массив указателей на соединения (для установки всем соединениям одинаковых параметров)
QMap<QString, QVariant> *userData = new QMap<QString, QVariant>;
QSqlQueryModel *userDataModel = new QSqlQueryModel();
QMap<QString, QVariant> *userLocalData = new QMap<QString, QVariant>;
QMap<QString, bool> *permissions = new QMap<QString, bool>;
QMap<QString, QVariant> *comSettings = new QMap<QString, QVariant>;
SStandardItemModel *clientPhoneTypesModel = new SStandardItemModel();
SSqlQueryModel *companiesModel = new SSqlQueryModel;
QSqlQueryModel *officesModel = new QSqlQueryModel;
QSqlQueryModel *warehousesModel = new QSqlQueryModel;
QSqlQueryModel *allUsersModel = new QSqlQueryModel;
QMap<int, QString> *allUsersMap = new QMap<int, QString>;
QSqlQueryModel *usersModel = new QSqlQueryModel;
QSqlQueryModel *managersModel = new QSqlQueryModel;
QSqlQueryModel *engineersModel = new QSqlQueryModel;
SSqlQueryModel *itemBoxesModel = new SSqlQueryModel;
SSqlQueryModel *repairBoxesModel = new SSqlQueryModel;
SSqlQueryModel *paymentSystemsModel = new SSqlQueryModel;
QStandardItemModel* clientsTypesList = new QStandardItemModel;
SSqlQueryModel* clientAdTypesList = new SSqlQueryModel;
QStandardItemModel *statusesModel = new QStandardItemModel();
QStandardItemModel *notifyStatusesModel = new QStandardItemModel();
SStandardItemModel *warrantyTermsModel = new SStandardItemModel();
QMap<int, QString> *warrantyTermsMap = new QMap<int, QString>;
QString commonComboBoxStyleSheet = "QComboBox {  border: 1px solid gray;  padding: 1px 18px 1px 3px;}\
        QComboBox::drop-down {  border: 0px;}\
        QComboBox::down-arrow{  image: url(down-arrow.png);  width: 16px;  height: 20px;}\
        QComboBox::hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}\
        QComboBox::down-arrow:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}";
QString commonComboBoxStyleSheetRed = "QComboBox {  border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}\
        QComboBox::drop-down {  border: 0px;}\
        QComboBox::down-arrow{  image: url(down-arrow.png);  width: 16px;  height: 20px;}\
        QComboBox::hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}\
        QComboBox::down-arrow:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}";
QString commonLineEditStyleSheet = "";
QString commonLineEditStyleSheetRed = "QLineEdit {  border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}";
QString commonTextEditStyleSheet = "";
QString commonTextEditStyleSheetRed = "QTextEdit {  border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}";
QString commonDateEditStyleSheet = "";
QString commonDateEditStyleSheetRed = "QDateEdit {  border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}";
SStandardItemModel *rejectReasonModel = new SStandardItemModel;
SStandardItemModel *priceColModel = new SStandardItemModel;
SStandardItemModel *itemUnitsModel = new SStandardItemModel;

//QWidget *modalWidget = nullptr;


#include "global.h"

void Global::errorMsg(const QString &text)
{
    qDebug() << "[Global] errorMsg() | text: " << text;
    errorPopupMsg(text);
//    appLog->appendRecord(text);
}

void Global::errorPopupMsg(const QString&)
{

}

void Global::throwError(const QSqlError &err, const QString &customErrorText)
{
    qDebug() << "[Global] throwError() | QSqlError: " << err.text() << ", custom text: " << customErrorText;
    throw ThrowType::ResultError;   // код ошибки выбран рандомно и ничего не значит
}

void Global::throwError(const int type, const QString&)
{
//    qDebug() << "[Global] throwError() | type: " << type << ", text: " << errorText;
    throw (ThrowType)type;
}

QMap<QString, QVariant> *loginCreds = new QMap<QString, QVariant>;
SComSettings *comSettings = new SComSettings;
SUserSettings *userDbData = new SUserSettings;
SSqlQueryModel *usersModel = new SSqlQueryModel();
SSqlQueryModel *allUsersModel = new SSqlQueryModel;
SSqlQueryModel *companiesModel = new SSqlQueryModel;
SSqlQueryModel *officesModel = new SSqlQueryModel;
SSqlQueryModel *managersModel = new SSqlQueryModel;
SSqlQueryModel *engineersModel = new SSqlQueryModel;
SAppLog *appLog;
QLocale sysLocale = QLocale::system();
SPermissions *permissions = new SPermissions;
SSqlQueryModel *paymentSystemsModel = new SSqlQueryModel;
SSqlQueryModel *repairBoxesModel = new SSqlQueryModel;
SStandardItemModel *warrantyTermsModel = new SStandardItemModel();
QMap<int, QString> *warrantyTermsMap = new QMap<int, QString>;
SStandardItemModel *notifyStatusesModel = new SStandardItemModel();

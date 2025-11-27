#include "global.h"

void Global::errorMsg(const QString &text)
{
    errorPopupMsg(text);
//    appLog->appendRecord(text);
}

void Global::errorPopupMsg(const QString&)
{

}

void Global::throwError(const QSqlError&, const QString&)
{
//    qDebug() << "[Global] throwError() | QSqlError: " << err.text() << ", custom text: " << customErrorText;
    throw ThrowType::ResultError;   // код ошибки выбран рандомно и ничего не значит
}

void Global::throwError(const int type, const QString&)
{
//    qDebug() << "[Global] throwError() | type: " << type << ", text: " << errorText;
    throw (ThrowType)type;
}

SComSettings *comSettings = new SComSettings;
SUserSettings *userDbData = new SUserSettings;
SSqlQueryModel *usersModel = new SSqlQueryModel();
SSqlQueryModel *officesModel = new SSqlQueryModel;
SAppLog *appLog;
QLocale sysLocale = QLocale::system();
SPermissions *permissions = new SPermissions;
SStandardItemModel *clientBinaryProperties;
SSqlQueryModel* clientAdTypesList = new SSqlQueryModel;
SStandardItemModel *priceColModel = new SStandardItemModel;
SSqlQueryModel *clientPhoneTypesModel = new SSqlQueryModel();

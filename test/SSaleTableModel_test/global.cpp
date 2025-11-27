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

SAppLog *appLog;
QLocale sysLocale = QLocale::system();
SComSettings *comSettings = new SComSettings;
SSqlQueryModel *companiesModel = new SSqlQueryModel;
SUserSettings *userDbData = new SUserSettings;
SSqlQueryModel *usersModel = new SSqlQueryModel();
SSqlQueryModel *allUsersModel = new SSqlQueryModel;
SSqlQueryModel *usersSalaryTaxesModel = new SSqlQueryModel();
QMap<int, QString> *allUsersMap = new QMap<int, QString>;
SPermissions *permissions = new SPermissions;
SStandardItemModel *engineersModel = new SStandardItemModel();
SSqlQueryModel *itemBoxesModel = new SSqlQueryModel;
SStandardItemModel *warrantyTermsModel = new SStandardItemModel();
SStandardItemModel *priceColModel = new SStandardItemModel;
SStandardItemModel *workPriceColModel = new SStandardItemModel;

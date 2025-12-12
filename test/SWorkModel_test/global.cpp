#include "global.h"

void Global::errorMsg(const QString &text)
{
    errorPopupMsg(text);
//    appLog->appendRecord(text);
}

void Global::errorPopupMsg(const QString&)
{

}

void Global::throwError(const QSqlError &err, const QString &customErrorText)
{
    qDebug() << "[Global] throwError() | QSqlError: " << err.text() << ", custom text: " << customErrorText;
    throw;
}

void Global::throwError(const int type, const QString &errorText)
{
    qDebug() << "[Global] throwError() | type: " << type << ", text: " << errorText;
    throw;
}

QMap<QString, QVariant> *loginCreds = new QMap<QString, QVariant>;
SUserSettings *userDbData = new SUserSettings;
SSqlQueryModel *usersModel = new SSqlQueryModel();
SSqlQueryModel *usersSalaryTaxesModel = new SSqlQueryModel();
SAppLog *appLog;
QLocale sysLocale = QLocale::system();
SStandardItemModel *warrantyTermsModel = new SStandardItemModel();

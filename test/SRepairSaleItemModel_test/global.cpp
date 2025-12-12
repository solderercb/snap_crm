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
QLocale sysLocale = QLocale::system();
SUserSettings *userDbData = new SUserSettings;
SSqlQueryModel *usersModel = new SSqlQueryModel;
SSqlQueryModel *allUsersModel = new SSqlQueryModel;
QMap<int, QString> *allUsersMap = new QMap<int, QString>;
SStandardItemModel *warrantyTermsModel = new SStandardItemModel();

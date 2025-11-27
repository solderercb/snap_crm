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

SUserSettings *userDbData = new SUserSettings;
SStandardItemModel *currencyListModel = new SStandardItemModel();
SStandardItemModel *engineersModel = new SStandardItemModel();
SStandardItemModel *repairDispatcherStatesModel = new SStandardItemModel();
SStandardItemModel *warrantyTermsModel = new SStandardItemModel();

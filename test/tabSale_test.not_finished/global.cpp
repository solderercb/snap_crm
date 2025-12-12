#include "global.h"

void Global::errorMsg(const QString &text)
{
    errorPopupMsg(text);
//    appLog->appendRecord(text);
}

void Global::errorPopupMsg(const QString &text)
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
SAppLog *appLog;
QLocale sysLocale = QLocale::system();
SComSettings *comSettings = new SComSettings;
SPermissions *permissions = new SPermissions;
SSqlQueryModel *companiesModel = new SSqlQueryModel;
SSqlQueryModel *paymentSystemsModel = new SSqlQueryModel;
SSqlQueryModel *clientPhoneTypesModel = new SSqlQueryModel();
SSqlQueryModel* clientAdTypesList = new SSqlQueryModel;
SStandardItemModel *priceColModel = new SStandardItemModel;
QString commonLineEditStyleSheet = "QLineEdit {  border: 1px solid gray;  padding: 1px 1px 1px 3px; background: #FFFFFF;}";
QString commonLineEditStyleSheetRed = "QLineEdit {  border: 1px solid red;  padding: 1px 1px 1px 3px; background: #FFD1D1;}";
QString commonDateEditStyleSheet = "QDateEdit {  border: 1px solid gray;  padding: 1px 18px 1px 3px; background: #FFFFFF;}\
        QDateEdit::drop-down {  border: 0px;}\
        QDateEdit::down-arrow{  image: url(:/icons/light/down-arrow.png);  width: 16px;  height: 20px;}\
        QDateEdit:!editable:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}\
        QDateEdit::down-arrow:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}";
QString commonDateEditStyleSheetRed = "QDateEdit {  border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}\
        QDateEdit::drop-down {  border: 0px;}\
        QDateEdit::down-arrow{  image: url(:/icons/light/down-arrow.png);  width: 16px;  height: 20px;}\
        QDateEdit:!editable:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}\
        QDateEdit::down-arrow:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}";
QString commonComboBoxStyleSheet = "QComboBox {  border: 1px solid gray;  padding: 1px 18px 1px 3px;}\
        QComboBox::drop-down {  border: 0px;}\
        QComboBox::down-arrow{  image: url(:/icons/light/down-arrow.png);  width: 16px;  height: 20px;}\
        QComboBox:!editable:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}\
        QComboBox::down-arrow:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}";
QString commonComboBoxStyleSheetRed = "QComboBox {  border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}\
        QComboBox::drop-down {  border: 0px;}\
        QComboBox::down-arrow{  image: url(:/icons/light/down-arrow.png);  width: 16px;  height: 20px;}\
        QComboBox:!editable:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}\
        QComboBox::down-arrow:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}";

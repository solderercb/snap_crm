#ifndef GLOBAL_H
#define GLOBAL_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QLocale>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QSettings>
#include "applog.h"
#include "models/scomsettings.h"
#include "models/susersettings.h"
#include "models/slocalsettings.h"
#include "models/slocalsettingsstructs.h"
#include "models/suseractivitymodel.h"
#include "models/sstandarditemmodel.h"
#include "models/ssqlquerymodel.h"
#include "models/spaymenttypesmodel.h"
#include "models/spermissions.h"
#ifdef QT_DEBUG
#include "clients4test.h"
#endif

namespace Global
{
    Q_NAMESPACE
    enum RepStateHeaders{Name, Id, Color, Terms, Contains, Actions, Roles};
    Q_ENUM_NS(RepStateHeaders)
    enum RepStateIds{GetIn, Diag, Negotiation, Agreed, InWork, PartsRequested, Ready, ReadyNoRepair, Returned, IssueNotAppeared, OfficeChange, NotAgreed, ReturnedNoRepair, OnApprovement, Approved, DiagFinished, ReturnedInCredit};
    Q_ENUM_NS(RepStateIds)
    // TODO: добавить модель данных действий для статуса (будет использоваться в редакторе статусов).
    enum RepStateActions{EditWorksParts = 1, ResetInformedStatus = 2, EditDiagSumm = 3, InformManager = 4, InformEngineer = 5, InformTimeout = 6, NoPayDiag = 7, SetEngineer = 8}; // значения больше 6 - мои доработки
    Q_ENUM_NS(RepStateActions)
    enum ClientInformStateIds{NotSet, Informed, NotResponse, NotAbailable, NotInformedOther};
    Q_ENUM_NS(ClientInformStateIds)
    enum PaymentSystemIds{Balance = -2, Card = -1, Cash = 0, Cashless = 1};
    Q_ENUM_NS(PaymentSystemIds)
    enum Reports {new_rep = 1, pko = 2, reject = 3, rko = 4, sticker1 = 5, sticker2 = 6, sticker3 = 7, rep_label = 8,
                  warranty = 9, works = 10, diag = 11, pn = 12, rn = 13, lost = 14, price1 = 15, invoice0 = 16,
                  invoice1 = 17, invoice2 = 18, invoice3 = 19, vatinvoice0 = 20, p_list0 = 21, w_list0 = 22,
                  new_cartridge = 23, sticker_cartridge = 24, issue_cartridge = 25, slip = 27, move = 28, buyout = 29,
                  repairs = 30, cartridges = 31,
                  tech_report,
                  not_impl = 256};
    Q_ENUM_NS(Reports)
    enum ThrowType {Debug = 0, QueryError = 1, TimeError = 2, ConnLost = 252, IntegrityError = 253, ConditionsError = 254, UserCanceled = 255};
    enum UserRoles {Admininstrator = 1, Engineer = 2, Manager = 3, Director = 4, SeniorEngineer = 5, SeniorManager = 6}; // роли по умолчанию
    Q_ENUM_NS(UserRoles)
}

extern QLocale sysLocale;
extern QVector<QSqlDatabase *> connections;    // массив указателей на соединения (для установки всем соединениям одинаковых параметров)
extern SUserSettings *userDbData;
extern SLocalSettings *localSettings;
extern t_userSettings *userLocalData;
extern SPermissions *permissions;
extern SComSettings *comSettings;
extern SSqlQueryModel *clientPhoneTypesModel;
extern SSqlQueryModel *companiesModel;
extern SSqlQueryModel *officesModel;
extern QSqlQueryModel *warehousesModel;
extern SSqlQueryModel *allUsersModel;
extern QMap<int, QString> *allUsersMap;
extern SSqlQueryModel *usersModel;
extern SSqlQueryModel *usersSalaryTaxesModel;
extern SSqlQueryModel *managersModel;
extern SSqlQueryModel *engineersModel;
extern SSqlQueryModel *itemBoxesModel;
extern SSqlQueryModel *repairBoxesModel;
extern SSqlQueryModel *paymentSystemsModel;
extern SPaymentTypesModel *receiptTypesModel;
extern SPaymentTypesModel *expenditureTypesModel;
extern SStandardItemModel* clientsTypesList;
extern SSqlQueryModel* clientAdTypesList;
extern SStandardItemModel *notifyStatusesModel;
extern SStandardItemModel *warrantyTermsModel;
extern QMap<int, QString> *warrantyTermsMap;
extern QString commonComboBoxStyleSheet;
extern QString commonComboBoxStyleSheetRed;
extern QString commonLineEditStyleSheet;
extern QString commonLineEditStyleSheetRed;
extern QString commonTextEditStyleSheet;
extern QString commonTextEditStyleSheetRed;
extern QString commonDateEditStyleSheet;
extern QString commonDateEditStyleSheetRed;
extern QString commonSpinBoxStyleSheet;
extern QString commonSpinBoxStyleSheetRed;
extern SStandardItemModel *rejectReasonModel;
extern SStandardItemModel *priceColModel;
extern SStandardItemModel *itemUnitsModel;
extern SAppLog *appLog;
extern SUserActivityModel *userActivityLog;
extern SStandardItemModel *clientBinaryProperties;
extern QSettings *debugOptions;
extern QMap<QString, QVariant> *debugLoginOptions;
extern SStandardItemModel* storeItemsIntReserveStatesModel;
extern QVector<QWidget*> tabList;
extern SStandardItemModel *cartridgeRepeatReason;
extern SStandardItemModel *cartridgeMaterialsListModel;
extern SStandardItemModel *currencyListModel;

//extern QSqlQueryModel *Model;
//extern QSqlQueryModel *Model;

void initSystemObjects();
void initUserDbData();
void initPermissions();
void initCompanies();
void initOffices();
void initGlobalModels();    // общие модели данных: организации, офисы, склады, сотрудники (все, менеджеры, инженеры) и др.

#endif // GLOBAL_H

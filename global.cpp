#include "global.h"
#include "com_sql_queries.h"
#include "models/sclientmodel.h"
#include "models/scartridgematerialsmodel.h"
#include "models/sstoreitemmodel.h"
#include "models/sofficemodel.h"
#include "models/stableclientsmodel.h"
#include "modules/purchasemanager/srequestsmodel.h"

QLocale sysLocale = QLocale::system();
QVector<QSqlDatabase *> connections;    // массив указателей на соединения (для установки всем соединениям одинаковых параметров)
SUserSettings *userDbData = new SUserSettings;
SLocalSettings *localSettings = new SLocalSettings();
t_userSettings *userLocalData = nullptr;
SPermissions *permissions = new SPermissions;
SComSettings *comSettings = new SComSettings;
SSqlQueryModel *clientPhoneTypesModel = new SSqlQueryModel();
SSqlQueryModel *companiesModel = new SSqlQueryModel;
SSqlQueryModel *officesModel = new SSqlQueryModel;
QSqlQueryModel *warehousesModel = new QSqlQueryModel;
SSqlQueryModel *allUsersModel = new SSqlQueryModel;
QMap<int, QString> *allUsersMap = new QMap<int, QString>;
SSqlQueryModel *usersModel = new SSqlQueryModel;
SSqlQueryModel *usersSalaryTaxesModel = new SSqlQueryModel;
SSqlQueryModel *managersModel = new SSqlQueryModel;
SSqlQueryModel *partRequestManagersModel;
SSqlQueryModel *engineersModel = new SSqlQueryModel;
SSqlQueryModel *itemBoxesModel = new SSqlQueryModel;
SSqlQueryModel *repairBoxesModel = new SSqlQueryModel;
SSqlQueryModel *paymentSystemsModel = new SSqlQueryModel;
SPaymentTypesModel *receiptTypesModel;
SPaymentTypesModel *expenditureTypesModel;
SStandardItemModel* clientsTypesList;
SSqlQueryModel* clientAdTypesList = new SSqlQueryModel;
SStandardItemModel *notifyStatusesModel = new SStandardItemModel();
SStandardItemModel *warrantyTermsModel = new SStandardItemModel();
QMap<int, QString> *warrantyTermsMap = new QMap<int, QString>;

// фактическая высота ComboBox'а будет на 4 пикселя больше; высоту нужно задать в styleSheet, т. к. Qt5 и Qt6 без этого параметра рисуют виджеты чуть-чуть разной высоты
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

QString commonLineEditStyleSheet = "QLineEdit {  border: 1px solid gray;  padding: 1px 1px 1px 3px; background: #FFFFFF;}";
QString commonLineEditStyleSheetRed = "QLineEdit {  border: 1px solid red;  padding: 1px 1px 1px 3px; background: #FFD1D1;}";
QString commonTextEditStyleSheet = "QTextEdit {  border: 1px solid gray;  padding: 1px 18px 1px 3px; background: #FFFFFF;}";
QString commonTextEditStyleSheetRed = "QTextEdit {  border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}";
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
QString commonSpinBoxStyleSheet = "QSpinBox, QDoubleSpinBox {  border: 1px solid gray;  padding: 1px 18px 1px 3px; background: #FFFFFF;}";
QString commonSpinBoxStyleSheetRed = "QSpinBox, QDoubleSpinBox {  border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}";
SStandardItemModel *rejectReasonModel = new SStandardItemModel;
SStandardItemModel *priceColModel = new SStandardItemModel;
SStandardItemModel *itemUnitsModel = new SStandardItemModel;
SAppLog *appLog;
SUserActivityModel *userActivityLog;
SStandardItemModel *clientBinaryProperties;
QSettings *debugOptions = nullptr;
QMap<QString, QVariant> *debugLoginOptions = nullptr;
SStandardItemModel* storeItemsIntReserveStatesModel = new SStandardItemModel;
QVector<QWidget*> tabList;
SStandardItemModel *cartridgeRepeatReason = new SStandardItemModel();
SStandardItemModel *cartridgeMaterialsListModel;
SStandardItemModel *currencyListModel = new SStandardItemModel();
SStandardItemModel *partRequestStatesListModel;
SStandardItemModel *partRequestPrioritiesListModel;

//QWidget *modalWidget = nullptr;

void initGlobalModels()
{
    comSettings->load();
    for (int i=0; i < comSettings->ascPhoneMask1.size(); i++)     // В ASC CRM (а точнее в DevExpress) маски ввода в lineEdit работают не так, как в Qt
        if (comSettings->ascPhoneMask1.at(i) == '0')              // В Qt: 0 - это необязательная цифра, а 9 — обязательная цифра. Чтобы работала проверка
        {                                                                           // введённого номера телефона (если в настройках установлен флаг обязательности), нужно изменить маску.
            qDebug() << "МАСКА ТЕЛЕФОНА 1!!!";                                      // (это заглушка для отладки)
            break;                                                                  // TODO: добавить хитрую подсказку для полей ввода номера, если обнаруживается такое несовпадение
        }
    for (int i=0; i < comSettings->ascPhoneMask2.size(); i++)
        if (comSettings->ascPhoneMask2.at(i) == '0')
        {
            qDebug() << "МАСКА ТЕЛЕФОНА 2!!!";
            break;
        }
    comSettings->ascPhoneMask1 =  comSettings->ascPhoneMask1.replace('0', '9'); // предупреждалка пусть орёт, но чтобы работала проверка, изменим маску внаглую
    comSettings->ascPhoneMask2 =  comSettings->ascPhoneMask2.replace('0', '9'); // предупреждалка пусть орёт, но чтобы работала проверка, изменим маску внаглую

    clientPhoneTypesModel->setQuery(QUERY_SEL_PHONE_TYPES, QSqlDatabase::database("connMain"));
    clientPhoneTypesModel->setObjectName("clientPhoneTypesModel");
//    clientPhoneTypesModel->setHorizontalHeaderLabels({"name", "id", "mask"});

    warehousesModel->setQuery(QUERY_SEL_WAREHOUSES(userDbData->currentOffice), QSqlDatabase::database("connMain"));
    warehousesModel->setObjectName("warehousesModel");
    allUsersModel->setQuery(QUERY_SEL_ALL_USERS, QSqlDatabase::database("connMain"));
    allUsersModel->setDisplayRoleColumn("username");
    allUsersModel->setObjectName("allUsersModel");
    for(int i = 0; i < allUsersModel->rowCount(); i++)
    {
        allUsersMap->insert(allUsersModel->record(i).value("id").toInt(), allUsersModel->record(i).value("username").toString());
    }
    usersModel->setQuery(QUERY_SEL_USERS, QSqlDatabase::database("connMain"));
    usersModel->setDisplayRoleColumn("username");
    usersModel->setObjectName("usersModel");
    usersSalaryTaxesModel->setQuery(QUERY_SEL_USERS_SALARY_TAXES, QSqlDatabase::database("connMain"));
    usersSalaryTaxesModel->setObjectName("usersSalaryTaxesModel");
    managersModel->setQuery(QUERY_SEL_MANAGERS, QSqlDatabase::database("connMain"));
    managersModel->setDisplayRoleColumn("username");
    managersModel->setObjectName("managersModel");
    engineersModel->setQuery(QUERY_SEL_ENGINEERS, QSqlDatabase::database("connMain"));
    engineersModel->setDisplayRoleColumn("username");
    engineersModel->setObjectName("engineersModel");
    itemBoxesModel->setQuery(QUERY_SEL_ITEM_BOXES(userDbData->currentOffice), QSqlDatabase::database("connMain"));
    itemBoxesModel->setObjectName("itemBoxesModel");
    repairBoxesModel->setQuery(QUERY_SEL_REPAIR_BOXES, QSqlDatabase::database("connMain"));
    repairBoxesModel->setObjectName("repairBoxesModel");

    // TODO: нужна прокси-модель для отображения платёжных систем в соответствии с правами пользователя
    // а также таблица payment_systems_users, содержащая права на видимость
    paymentSystemsModel->setQuery(QUERY_SEL_PAYMENT_SYSTEMS, QSqlDatabase::database("connMain"));
    paymentSystemsModel->setObjectName("paymentSystemsModel");

    receiptTypesModel = new SPaymentTypesModel(SPaymentTypesModel::Type::PKO);
    receiptTypesModel->setObjectName("receiptTypesModel"); // TODO: возможно, тоже нужны прокси-модели для отображения типов ПКО и РКО в соответствии с правами пользователя
    expenditureTypesModel = new SPaymentTypesModel(SPaymentTypesModel::Type::RKO);
    expenditureTypesModel->setObjectName("expenditureTypesModel");

    clientAdTypesList->setQuery(QUERY_SEL_CLIENT_AD_TYPES, QSqlDatabase::database("connMain"));
    clientAdTypesList->setObjectName("clientAdTypesList");

    clientsTypesList = SClientModel::categoriesList();

    QVector<QString> notifyStatusesList = {"---", QObject::tr("Клиент оповещён"), QObject::tr("Клиент не отвечает"), QObject::tr("Клиент не доступен"), QObject::tr("Не оповещён прочее")};
    for (int i=0; i<notifyStatusesList.size(); i++)
    {
        QList<QStandardItem*> *notifyStatusSelector = new QList<QStandardItem*>();
        *notifyStatusSelector << new QStandardItem(notifyStatusesList.at(i)) << new QStandardItem(QString::number(i));
        notifyStatusesModel->appendRow(*notifyStatusSelector);
    }
    notifyStatusesModel->setHorizontalHeaderLabels({"name","id"});

    // TODO: В АСЦ в списке сроков гарантии также присутствуют "Согласно гарантии производителя", "Согласно ЗоЗПП" и "Согласно ФГТ", но, похоже, их выбор не реализован
    QVector<QString> warrantyTermsList = {QObject::tr("нет"), QObject::tr("7 дней"), QObject::tr("14 дней"), QObject::tr("1 мес"), QObject::tr("2 мес"), QObject::tr("3 мес"), QObject::tr("4 мес"), QObject::tr("6 мес"), QObject::tr("1 год"), QObject::tr("2 года"), QObject::tr("3 года")};
    int warrantyTerms[] = {0, 7, 14, 31, 62, 93, 124, 186, 365, 730, 1095};
    // Модель для комбобоксов при редактировании, а QMap для отображения
    for (int i=0; i<warrantyTermsList.size(); i++)
    {
        QList<QStandardItem*> *warrantyTermSelector = new QList<QStandardItem*>();
        *warrantyTermSelector << new QStandardItem(warrantyTermsList.at(i)) << new QStandardItem(QString::number(warrantyTerms[i]));
        warrantyTermsModel->appendRow(*warrantyTermSelector);
        warrantyTermsMap->insert(warrantyTerms[i], warrantyTermsList.at(i));
    }
    warrantyTermsModel->setObjectName("warrantyTermsModel");
    warrantyTermsModel->setHorizontalHeaderLabels({"name","days"});


    QVector<QString> rejectReasonList = {QObject::tr("отказ от ремонта"), QObject::tr("ремонт не возможен"), QObject::tr("ремонт не возможен из-за отсутствия запчастей"), QObject::tr("ремонт не рентабелен"), QObject::tr("неисправносте не проявилась"), QObject::tr("другие причины")};
    rejectReasonModel->setProperty("other_reject_reason", 5); // более элегантный способ не придумал (такой, чтобы задавать id причины отказа в "одном месте" и чтобы это не поломалось при переводе)
    QList<QStandardItem*> *rejectReasonSelector;
    for (int i=0; i<rejectReasonList.size(); i++)
    {
        rejectReasonSelector = new QList<QStandardItem*>();
        *rejectReasonSelector << new QStandardItem(rejectReasonList.at(i)) << new QStandardItem(QString::number(i));
        rejectReasonModel->appendRow(*rejectReasonSelector);
    }
    rejectReasonModel->setObjectName("rejectReasonModel");
    rejectReasonModel->setHorizontalHeaderLabels({"name","id"});

    priceColModel = SStoreItemModel::priceOptionsList();

    QVector<QString> itemUnitsList = {QObject::tr("шт"), QObject::tr("г"), QObject::tr("м"), QObject::tr("см"), QObject::tr("л")};
    QVector<QString> itemUnitsIdsList = {"1", "2", "3", "4", "5"};
    QList<QStandardItem*> *itemUnitsSelector;
    for (int i=0; i<itemUnitsList.size(); i++)
    {
        itemUnitsSelector = new QList<QStandardItem*>();
        *itemUnitsSelector << new QStandardItem(itemUnitsList.at(i)) << new QStandardItem(itemUnitsIdsList.at(i));
        itemUnitsModel->appendRow(*itemUnitsSelector);
    }
    itemUnitsModel->setObjectName("itemUnitsModel");
    itemUnitsModel->setHorizontalHeaderLabels({"name", "id"});

    QVector<QString> storeItemsIntReserveStatesList = {QObject::tr("Ожидание"), QObject::tr("Товар выдан инженеру"), QObject::tr("Товар установлен"), QObject::tr("Товар установлен, устройство выдано"), QObject::tr("Архив"), QObject::tr("Отклонена")};
    for (int i=0; i<storeItemsIntReserveStatesList.size(); i++)
    {
        QList<QStandardItem*> *storeItemsIntReserveStateSelector = new QList<QStandardItem*>();
        *storeItemsIntReserveStateSelector << new QStandardItem(storeItemsIntReserveStatesList.at(i)) << new QStandardItem(QString::number(i));
        storeItemsIntReserveStatesModel->appendRow(*storeItemsIntReserveStateSelector);
    }
    storeItemsIntReserveStatesModel->setObjectName("storeItemsIntReserveStatesModel");
    storeItemsIntReserveStatesModel->setHorizontalHeaderLabels({"name", "id"});

    QVector<QString> cartridgeRepeatReasonList = {QObject::tr("Гарантийный ремонт"), QObject::tr("Ранее было в ремонте")};
    QVector<QString> cartridgeRepeatReasonIdsList = {"1", "2"};
    QList<QStandardItem*> *cartridgeRepeatReasonSelector;
    for (int i=0; i<cartridgeRepeatReasonList.size(); i++)
    {
        cartridgeRepeatReasonSelector = new QList<QStandardItem*>();
        *cartridgeRepeatReasonSelector << new QStandardItem(cartridgeRepeatReasonList.at(i)) << new QStandardItem(cartridgeRepeatReasonIdsList.at(i));
        cartridgeRepeatReason->appendRow(*cartridgeRepeatReasonSelector);
    }
    cartridgeRepeatReason->setObjectName("cartridgeRepeatReason");
    cartridgeRepeatReason->setHorizontalHeaderLabels({"name", "id"});

    cartridgeMaterialsListModel = SCartridgeMaterialsModel::materialsList();

    QVector<QString> currencyList = {QObject::tr("Российский рубль"), QObject::tr("Доллар США"), QObject::tr("Украинская гривна"),
                                     QObject::tr("Белорусский рубль"), QObject::tr("Новый израильский шекель"), QObject::tr("Молдавкий лей"),
                                     QObject::tr("Польский злотый"), QObject::tr("Китайский юань"), QObject::tr("Болгарский лев"),
                                     QObject::tr("Узбекский сум"), QObject::tr("Казахста́нский тенге́")};
//    QVector<QString> currencyIdsList = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    QVector<QString> currencyCodeList = {"RUB", "USD", "UAH", "BYN", "ILS", "MDL", "PLN", "CNY", "BGN", "UZS", "KZT"};
    QList<QStandardItem*> *currencySelector;
    for (int i=0; i<currencyList.size(); i++)
    {
        currencySelector = new QList<QStandardItem*>();
        *currencySelector << new QStandardItem(currencyList.at(i)) << new QStandardItem(currencyCodeList.at(i));
        currencyListModel->appendRow(*currencySelector);
    }
    currencyListModel->setObjectName("currency");
    currencyListModel->setHorizontalHeaderLabels({"name", "id"});

    partRequestStatesListModel = SPartRequest::statesList();
    partRequestPrioritiesListModel = SPartRequest::prioritiesList();
    partRequestManagersModel = SPartRequest::managersList();

#ifdef QT_DEBUG
    initClients4Test();
#endif
}

void initUserDbData()
{
    userDbData->load(QSqlDatabase::database("connMain").userName());
}

void initPermissions()
{
    permissions->load(userDbData->roles);
}

void initCompanies()    // Список компаний.
{
    companiesModel->setQuery(QUERY_SEL_COMPANIES, QSqlDatabase::database("connMain"));
    companiesModel->setObjectName("companiesModel");
}

void initOffices()      // Список офисов
{
    officesModel->setQuery(QUERY_SEL_OFFICES(userDbData->company), QSqlDatabase::database("connMain"));
    officesModel->setObjectName("officesModel");
}

void initSystemObjects()
{
    userActivityLog = new SUserActivityModel();
//    enum ClientBinaryOption{Company = 1, Regular = 2, Broker = 4, IgnoreCalls = 8, PreferCashless = 16, TakeLong = 32, Supplier = 64, SaleOrReturn = 128, BalanceEnabled = 256, Bad = 512, Archived = 1024};
    clientBinaryProperties = new SStandardItemModel();
    QVector<QString> NamesList = {QObject::tr("Юридическое лицо", "company"), QObject::tr("Постоянный клиент", "regular"),
                                  QObject::tr("Посредник", "broker (middleman)"), QObject::tr("Игнорирует звонки", "ignores calls"),
                                  QObject::tr("Предпочитает безнал", "prefer cashless"), QObject::tr("Не забирает технику в срок", "take out not in time"),
                                  QObject::tr("Является поставщиком", "supplier"), QObject::tr("Даёт товар под реализацию", "sale or return"),
                                  QObject::tr("Баланс", "client balance"), QObject::tr("Проблемный", "problematic client"), QObject::tr("Архивный", "archived")};
    QVector<QString> NamesListShort = {QObject::tr("юр.", "company (short)"), QObject::tr("пост.", "regular (short)"),
                                  QObject::tr("поср.", "broker (middleman) (short)"), QObject::tr("игн. зв.", "ignores calls (short)"),
                                  QObject::tr("безнал", "prefer cashless (short)"), QObject::tr("долго забир.", "take out not in time (short)"),
                                  QObject::tr("поставщ.", "supplier (short)"), QObject::tr("реализ.", "sale or return (short)"),
                                  QObject::tr("бал.", "client balance (short)"), QObject::tr("проблемный", "problematic client (short)"), QObject::tr("арх.", "archived (short)")};
    QVector<int> bitfieldsList = {SClientModel::Company, SClientModel::Regular,
                                  SClientModel::Broker, SClientModel::IgnoreCalls,
                                  SClientModel::PreferCashless, SClientModel::TakeLong,
                                  SClientModel::Supplier, SClientModel::SaleOrReturn,
                                  SClientModel::BalanceEnabled, SClientModel::Bad, SClientModel::Archived};
    QVector<QString> DBFieldsList = {"type", "is_regular",
                                     "is_agent", "ignore_calls",
                                     "prefer_cashless", "take_long",
                                     "is_dealer", "is_realizator",
                                     "balance_enable", "is_bad", "state"};
    QList<QStandardItem*> *selector;
    for (int i=0; i<NamesList.size(); i++)
    {
        selector = new QList<QStandardItem*>();
        *selector << new QStandardItem(NamesList.at(i)) << new QStandardItem(QString::number(bitfieldsList.at(i))) << new QStandardItem(DBFieldsList.at(i)) << new QStandardItem(NamesListShort.at(i));
        clientBinaryProperties->appendRow(*selector);
    }
    clientBinaryProperties->setObjectName("clientBinaryProperties");
    clientBinaryProperties->setHorizontalHeaderLabels({"name", "bitfield", "dbField, shortName"});
}
